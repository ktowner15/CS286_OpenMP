#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <sstream>
#include <list>
#include <string>

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
	
#pragma omp parallel
	{
	}
	
	stopwatch S1;
	S1.start();

	/////////////////////////////////////////////////////////////////////
	///////////////////////  YOUR CODE HERE       ///////////////////////
	/*NONTHREADED VERSION*/
		int maxVal = 0;
		int avgs[rows][cols];				//Array to hold avg values
		/*Test code for printing out generated array*/
		cout << "Datavals array: " << endl;
		for(int k = 0; k < rows; k++){		//k = row index, m = cols index  (access is data[k][m])
			for(int m = 0; m < cols; m++){
				cout << data[k][m] << " ";
			}
			cout << endl;
		}
		cout << endl;
		/*Test code for printing neighborhood avg of each cell*/
		double newArray[rows + 2][ cols + 2];

		cout << "Creating new array... " << endl;
		for(int a = 0; a < rows + 2; a++){
			for(int b = 0; b < cols + 2; b++){
				newArray[a][b] = -1;
			}
		}
		
		// cout << "New array: " << endl;
		// for(int c = 0; c < rows + 2; c++){
		// 	for(int d = 0; d < cols + 2; d++){
		// 		cout << newArray[c][d] << " ";
		// 	}
		// 	cout << endl;
		// }
		

		cout << "Filling new array... " << endl;
		for(int e = 1; e < rows + 1; e++){
			for(int f = 1; f < cols + 1; f++){
				newArray[e][f] = data[e - 1][f - 1];
			}
		}

		// cout << "New array: " << endl;
		// for(int c = 0; c < rows + 2; c++){
		// 	for(int d = 0; d < cols + 2; d++){
		// 		cout << newArray[c][d] << " ";
		// 	}
		// 	cout << endl;
		// }

		cout << "Printing just the datavals from new array: " << endl;
		for(int e = 1; e < rows + 1; e++){
			for(int f = 1; f < cols + 1; f++){
				cout << data[e - 1][f - 1] << " ";
			}
			cout << endl;
		}

		cout << "Printing edge status of each cell: " << endl;
		for(int e = 1; e < rows + 1; e++){
			for(int f = 1; f < cols + 1; f++){
				if(newArray[e-1][f+1] == -1 && newArray[e][f+1] != -1 					/*<-----------The comparisons start at newArray[e-1][f+1], which is the node's upper right neighbor, and continues clockwise*/
				    && newArray[e+1][f+1] != -1 && newArray[e+1][f] != -1 
					&& newArray[e+1][f-1] == -1 && newArray[e][f-1] == -1 
					&& newArray[e-1][f-1] == -1 && newArray[e-1][f] == -1){	//Upper-left edge node	(CORRECT)
					cout << "ul" << " ";
				}
				else if(newArray[e-1][f+1] == -1 && newArray[e][f+1] != -1 
				    && newArray[e+1][f+1] != -1 && newArray[e+1][f] != -1 
					&& newArray[e+1][f-1] != -1 && newArray[e][f-1] != -1 
					&& newArray[e-1][f-1] == -1 && newArray[e-1][f] == -1){	//upper edge (CORRECT)
					cout << "up" << " ";
				}
				else if(newArray[e-1][f+1] == -1 && newArray[e][f+1] == -1 
				    && newArray[e+1][f+1] == -1 && newArray[e+1][f] != -1 
					&& newArray[e+1][f-1] != -1 && newArray[e][f-1] != -1 
					&& newArray[e-1][f-1] == -1 && newArray[e-1][f] == -1){	//Upper-right edge node (CORRECT)
					cout << "ur" << " ";
				}
				else if(newArray[e-1][f+1] != -1 && newArray[e][f+1] != -1 
				    && newArray[e+1][f+1] != -1 && newArray[e+1][f] != -1 
					&& newArray[e+1][f-1] == -1 && newArray[e][f-1] == -1 
					&& newArray[e-1][f-1] == -1 && newArray[e-1][f] != -1){	//left edge (CORRECT)
					cout << "le" << " ";
				}
				else if(newArray[e+1][f] != -1 && newArray[e+1][f+1] != -1 
				        && newArray[e][f+1] != -1 && newArray[e-1][f+1] != -1 
						&& newArray[e-1][f] != -1 && newArray[e-1][f-1] != -1 
						&& newArray[e][f-1] != -1 && newArray[e+1][f] != -1){	//Interior node	(CORRECT)
					cout << "in" << " ";
				}
				else if(newArray[e][f+1] == -1){	//right edge	FIXME: left off here (changing if/else comparisons to label cells more accurately.)
					cout << "ri" << " ";
				}
				else if(newArray[e+1][f-1] == -1 && newArray[e][f-1] == -1 && newArray[e+1][f] == -1){	//lower-left edge node
					cout << "ll" << " ";
				}
				else if(newArray[e+1][f] == -1){	//lower edge
					cout << "lo" << " ";
				}
				else if(newArray[e-1][f-1] == -1 && newArray[e][f-1] == -1 && newArray[e-1][f] == -1){	//lower-right edge node
					cout << "lr" << " ";
				}
			}
			cout << endl;
		}

		// for(int){
		// 	for(){

		// 	}
		// }


	/*END OF NONTHREADED VERSION*/
	/////////////////////////////////////////////////////////////////////
    cout << "ASDFGASDFSADF"<<endl;
	
	S1.stop();
	
	// print out the max value here
	
	cerr << "elapsed time: " << S1.getTime( ) << endl;
}


