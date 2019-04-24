#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <sstream>
#include <list>
#include <string>
#include <vector>

using namespace std;

// a class to get more accurate time

class stopwatch{
	
private:
	double elapsedTime;
	double startedTime;
	bool timing;
	//returns current time in seconds
	double current_time( ) 
	{
		timeval tv;
		gettimeofday(&tv, NULL);
		double rtn_value = (double) tv.tv_usec;
		rtn_value /= 1e6;
		rtn_value += (double) tv.tv_sec;
		return rtn_value;
	}
	
public:
	stopwatch( ): elapsedTime( 0 ), startedTime( 0 ), timing( false )
	{
		
	}
	
	void start( )
	{
		if( !timing )
		{
			timing = true;
			startedTime = current_time( );
		}
	}
	
	void stop( )
	{
		if( timing )
		{
			elapsedTime +=  current_time( )-startedTime;
			timing = false;
		}
	}
	
	void resume( )
	{
		start( );
	}
	
	void reset( )
	{
		elapsedTime = 0;
		startedTime = 0;
		timing = false;
	}
	
	double getTime( )
	{
		return elapsedTime;
	}
};

struct average{
	double value;
	int rowIndex;
	int colIndex;
};


// function takes an array pointer, and the number of rows and cols in the array, and 
// allocates and intializes the two dimensional array to a bunch of random numbers

void makeRandArray( unsigned int **& data, unsigned int rows, unsigned int cols, unsigned int seed )
{
	// allocate the array
	data = new unsigned int*[ rows ];
	for( unsigned int i = 0; i < rows; i++ )
	{
		data[i] = new unsigned int[ cols ];
	}
	
	// seed the number generator
	// you should change the seed to get different values
	srand( seed );
	
	// populate the array
	
	for( unsigned int i = 0; i < rows; i++ )
		for( unsigned int j = 0; j < cols; j++ )
		{
			data[i][j] = rand() % 10000 + 1; // number between 1 and 10000
		}
	
}

void getDataFromFile( unsigned int **& data, char fileName[], unsigned int &rows, unsigned int &cols )
{
	ifstream in;
	in.open( fileName );
	if( !in )
	{
		cerr << "error opening file: " << fileName << endl;
		exit( -1 );
	}
	
	in >> rows >> cols;
	data = new unsigned int*[ rows ];
	for( unsigned int i = 0; i < rows; i++ )
	{
		data[i] = new unsigned int[ cols ];
	}
	
	// now read in the data
	
	for( unsigned int i = 0; i < rows; i++ )
		for( unsigned int j = 0; j < cols; j++ )
		{
			in >> data[i][j];
		}
	
}


int main( int argc, char* argv[] ) 
{
	if( argc < 3 )
	{
		cerr<<"Usage: " << argv[0] << " [input data file] [num of threads to use] " << endl;
		
		cerr<<"or" << endl << "Usage: "<< argv[0] << " rand [num of threads to use] [num rows] [num cols] [seed value]" << endl;
                exit( 0 );
        }
	
	// read in the file
	unsigned int rows, cols, seed;
	unsigned int numThreads;
	unsigned int ** data;
	// convert numThreads to int
	{
		stringstream ss1;
		ss1 << argv[2];
		ss1 >> numThreads;
	}
	
	string fName( argv[1] );
	if( fName == "rand" )
	{
		{
			stringstream ss1;
			ss1 << argv[3];
			ss1 >> rows;
		}
		{
			stringstream ss1;
			ss1 << argv[4];
			ss1 >> cols;
		}
		{
			stringstream ss1;
			ss1 << argv[5];
			ss1 >> seed;
		}
		makeRandArray( data, rows, cols, seed );
	}
	else
	{
		getDataFromFile( data,  argv[1], rows, cols );
	}
	
		cerr << "data: " << endl;
	 for( unsigned int i = 0; i < rows; i++ )
	 {
	 for( unsigned int j = 0; j < cols; j++ )
	 {
	 cerr << "i,j,data " << i << ", " << j << ", ";
	 cerr << data[i][j] << " ";
	 }
	 cerr << endl;
	 }
	 cerr<< endl;
	
	// tell omp how many threads to use
	omp_set_num_threads( numThreads );
	

	stopwatch S1;
	S1.start();
	
	vector<average> avgs;					//Vector for holding averages
#pragma omp parallel
	{
		for(int a = 0; a < rows; a++){
				for(int b = 0; b < cols; b++){				//b = x coordinate, a = y coordinate (this comment is to aid logical mental conception)
					if(a==0&&b==0){
						//cout << "ul\t";//upper left
						average tempUL;
						
							tempUL.value = (double)(data[b+1][a] + data[b+1][a+1] + data[b][a+1]) / 3;	//Calculate average
							tempUL.rowIndex = a;
							tempUL.colIndex = b;
						
						avgs.push_back(tempUL);
					}
					else if(a==0&&b!=0&&b!=cols-1){
						//cout << "us\t";//upper side
						average tempUS;
						
							tempUS.value = (double)(data[b+1][a] + data[b+1][a+1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a]) / 5;	//FIXME: breaking at this line (index out of bounds?)
							tempUS.rowIndex = a;
							tempUS.colIndex = b;
						
						avgs.push_back(tempUS);
					}
					else if(a==0&&b==cols - 1){
						//cout << "ur\t";//upper right
						average tempUR;
						
							tempUR.value = (double)(data[b][a+1] + data[b-1][a+1] + data[b-1][a]) / 3;
							tempUR.rowIndex = a;
							tempUR.colIndex = b;
						
						avgs.push_back(tempUR);
					}
					else if(a!=0&&a!=cols-1&&b==0){
						//cout << "ls\t";//left side
						average tempLS;
						
							tempLS.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b+1][a+1] + data[b][a+1]) / 5;
							tempLS.rowIndex = a;
							tempLS.colIndex = b;
						
						avgs.push_back(tempLS);
					}
					else if(a!=0&&a!=cols-1&&b!=0&&b!=cols-1){
						//cout << "i\t";//interior
						average tempI;
						
							tempI.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b+1][a+1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a] + data[b-1][a-1]) / 8;
							tempI.rowIndex = a;
							tempI.colIndex = b;
						
						avgs.push_back(tempI);
					}
					else if(a!=0&&a!=cols-1&&b==cols - 1){
						//cout << "rs\t";//right side
						average tempR;
						
							tempR.value = (double)(data[b][a-1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a] + data[b-1][a-1]) / 5;
							tempR.rowIndex = a;
							tempR.colIndex = b;
						
						avgs.push_back(tempR);
					}
					else if(a==rows - 1 && b==0){
						//cout << "ll\t";//lower left
						average tempLL;
						
							tempLL.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a]) / 3;
							tempLL.rowIndex = a;
							tempLL.colIndex = b;
						
						avgs.push_back(tempLL);
					}
					else if(a==rows - 1 && b!=0&&b!=cols-1){
						//cout << "bs\t";//bottom side
						average tempBS;
						
							tempBS.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b-1][a] + data[b-1][a-1]) / 5;
							tempBS.rowIndex = a;
							tempBS.colIndex = b;
						
						avgs.push_back(tempBS);
					}
					else if(a==rows - 1 && b == cols - 1){
						//cout << "lr\t";//lower right
						average tempLS;
						
							tempLS.value = (double)(data[b][a-1] + data[b-1][a] + data[b-1][a-1]) / 3;
							tempLS.rowIndex = a;
							tempLS.colIndex = b;
						
						avgs.push_back(tempLS);
					}
					else{
						//cout << data[a][b] << " ";
					}
				}
				//cout << endl;
			}
	}
	
	

	/////////////////////////////////////////////////////////////////////
	///////////////////////  YOUR CODE HERE       ///////////////////////
	// ofstream ofs;
	// ofs.open("TestAverages.txt");
	// if(!ofs.is_open()){
	// 	cerr << "Error opening file, try again..." << endl;
	// 	return -1;
	// }
	/*NONTHREADED VERSION, for testing*/
		//vector<average> avgs;					//Vector for holding averages
		//int avgs[rows][cols];				//Array to hold avg values

		/*Test code for printing out generated array*/
		// cout << "Datavals array: " << endl;
		// for(int k = 0; k < rows; k++){		//k = row index, m = cols index  (access is data[k][m])
		// 	for(int m = 0; m < cols; m++){
		// 		cout << data[k][m] << " ";
		// 	}
		// 	cout << endl;
		// }
		// cout << endl;
		
		// cout << "Printing edge status of each node: " << endl;
		// for(int a = 0; a < rows; a++){
		// 	for(int b = 0; b < cols; b++){
		// 		if(a%(rows - 1) == 0 && b%(cols - 1) == 0){
		// 			cout << "c ";						//corner node
		// 		}
		// 		else if(a==0 || a == rows - 1 || b == 0 || b == cols - 1){
		// 			cout << "s ";						//side nodes
		// 		}
				
		// 		else{
		// 			cout << "i ";						//interior node
		// 		}
		// 	}
		// 	cout << endl;
		// }
		// cout << endl;

		//cout << "Printing averages of each node: " << endl;
		//cout << endl;

		// cout << "Printing vector of averages: " << endl;
		// for(int i = 0; i < avgs.size(); i++){
		// 	cout << "Number of nodes printed: " << i + 1 << endl
		// 	     << "Value: " <<  avgs[i].value << endl
		// 	     << "Index: " << "data[" << avgs[i].rowIndex << "][" << avgs[i].colIndex << "]" << endl << endl; 
		// }

		cout << "Calculating highest average: " << endl
		     << "Highest average: ";
		average max = avgs[0];
		for(int i = 0; i < avgs.size(); i++){
			if(avgs[i].value >= max.value){
				max = avgs[i];
			}
			else{
				max = max;
			}
		}
		cout << max.value << endl 
		     << "Index: data[" << max.rowIndex << "][" << max.colIndex << "]" 
			 << endl << endl;

		// for(int i = 0; i < avgs.size(); i++){
		// 	ofs << "Node " << i+1 << ": " << endl
		// 	    << avgs[i].value << endl << endl;
		// }

	/////////////////////////////////////////////////////////////////////
    cout << "ASDFGASDFSADF"<<endl;
	
	S1.stop();
	
	// print out the max value here
	
	cerr << "elapsed time: " << S1.getTime( ) << endl;
}

/*Notes:

corners at indexes [0][0], [0][cols], [rows][0], [rows][cols]

*/


