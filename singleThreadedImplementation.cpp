/////////////////////////////////////////////////////////////////////
	///////////////////////  YOUR CODE HERE       ///////////////////////
	ofstream ofs;
	ofs.open("TestAverages.txt");
	if(!ofs.is_open()){
		cerr << "Error opening file, try again..." << endl;
		return -1;
	}
	/*NONTHREADED VERSION, for testing*/
		vector<average> avgs;					//Vector for holding averages
		int maxVal = 0;
		//int avgs[rows][cols];				//Array to hold avg values

		/*Test code for printing out generated array*/
		cout << "Datavals array: " << endl;
		for(int k = 0; k < rows; k++){		//k = row index, m = cols index  (access is data[k][m])
			for(int m = 0; m < cols; m++){
				cout << data[k][m] << " ";
			}
			cout << endl;
		}
		cout << endl;
		
		cout << "Printing edge status of each node: " << endl;
		for(int a = 0; a < rows; a++){
			for(int b = 0; b < cols; b++){
				if(a%(rows - 1) == 0 && b%(cols - 1) == 0){
					cout << "c ";						//corner node
				}
				else if(a==0 || a == rows - 1 || b == 0 || b == cols - 1){
					cout << "s ";						//side nodes
				}
				
				else{
					cout << "i ";						//interior node
				}
			}
			cout << endl;
		}
		cout << endl;

		cout << "Printing averages of each node: " << endl;
		for(int a = 0; a < rows; a++){
			for(int b = 0; b < cols; b++){				//b = x coordinate, a = y coordinate (this comment is to aid logical mental conception)
				if(a==0&&b==0){
					cout << "ul\t";//upper left
					average tempUL;
					tempUL.value = (double)(data[b+1][a] + data[b+1][a+1] + data[b][a+1]) / 3;	//Calculate average
					tempUL.rowIndex = a;
					tempUL.colIndex = b;
					avgs.push_back(tempUL);
				}
				else if(a==0&&b!=0&&b!=cols-1){
					cout << "us\t";//upper side
					average tempUS;
					tempUS.value = (double)(data[b+1][a] + data[b+1][a+1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a]) / 5;	//FIXME: breaking at this line (index out of bounds?)
					tempUS.rowIndex = a;
					tempUS.colIndex = b;
					avgs.push_back(tempUS);
				}
				else if(a==0&&b==cols - 1){
					cout << "ur\t";//upper right
					average tempUR;
					tempUR.value = (double)(data[b][a+1] + data[b-1][a+1] + data[b-1][a]) / 3;
					tempUR.rowIndex = a;
					tempUR.colIndex = b;
					avgs.push_back(tempUR);
				}
				else if(a!=0&&a!=cols-1&&b==0){
					cout << "ls\t";//left side
					average tempLS;
					tempLS.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b+1][a+1] + data[b][a+1]) / 5;
					tempLS.rowIndex = a;
					tempLS.colIndex = b;
					avgs.push_back(tempLS);
				}
				else if(a!=0&&a!=cols-1&&b!=0&&b!=cols-1){
					cout << "i\t";//interior
					average tempI;
					tempI.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b+1][a+1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a] + data[b-1][a-1]) / 8;
					tempI.rowIndex = a;
					tempI.colIndex = b;
					avgs.push_back(tempI);
				}
				else if(a!=0&&a!=cols-1&&b==cols - 1){
					cout << "rs\t";//right side
					average tempR;
					tempR.value = (double)(data[b][a-1] + data[b][a+1] + data[b-1][a+1] + data[b-1][a] + data[b-1][a-1]) / 5;
					tempR.rowIndex = a;
					tempR.colIndex = b;
					avgs.push_back(tempR);
				}
				else if(a==rows - 1 && b==0){
					cout << "ll\t";//lower left
					average tempLL;
					tempLL.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a]) / 3;
					tempLL.rowIndex = a;
					tempLL.colIndex = b;
					avgs.push_back(tempLL);
				}
				else if(a==rows - 1 && b!=0&&b!=cols-1){
					cout << "bs\t";//bottom side
					average tempBS;
					tempBS.value = (double)(data[b][a-1] + data[b+1][a-1] + data[b+1][a] + data[b-1][a] + data[b-1][a-1]) / 5;
					tempBS.rowIndex = a;
					tempBS.colIndex = b;
					avgs.push_back(tempBS);
				}
				else if(a==rows - 1 && b == cols - 1){
					cout << "lr\t";//lower right
					average tempLS;
					tempLS.value = (double)(data[b][a-1] + data[b-1][a] + data[b-1][a-1]) / 3;
					tempLS.rowIndex = a;
					tempLS.colIndex = b;
					avgs.push_back(tempLS);
				}
				else{
					cout << data[a][b] << " ";
				}
			}
			cout << endl;
		}
		cout << endl;

		cout << "Printing vector of averages: " << endl;
		for(int i = 0; i < avgs.size(); i++){
			cout << "Number of nodes printed: " << i + 1 << endl
			     << "Value: " <<  avgs[i].value << endl
			     << "Index: " << "data[" << avgs[i].rowIndex << "][" << avgs[i].colIndex << "]" << endl << endl; 
		}

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

		for(int i = 0; i < avgs.size(); i++){
			ofs << "Node " << i+1 << ": " << endl
			    << avgs[i].value << endl << endl;
		}

	/////////////////////////////////////////////////////////////////////