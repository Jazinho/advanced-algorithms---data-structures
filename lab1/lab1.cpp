//============================================================================
// Name        : lab1.cpp
// Author      : JP
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
	ifstream dataFile;
  dataFile.open ("data.csv");
	string line;
	vector<vector<int> > edgesVector;

	if (dataFile.is_open()){
		while ( getline (dataFile,line) ){
			int i;
			vector<int> values;
			stringstream ss(line);

	    while (ss >> i){
					values.push_back(i);

	        if (ss.peek() == ','){
						ss.ignore();
					}
	    }

			edgesVector.push_back(values);
    }

		for (int k=0; k< edgesVector.size(); k++){
			for (int l=0; l< edgesVector.at(k).size(); l++){
        std::cout << edgesVector.at(k).at(l);
			}
			cout << endl;
		}

		dataFile.close();
	} else {
		cout << "Cannot open file!";
		return 1;
	}

	int edges[7][3]={
			{0,1,2},
			{1,2,5},
			{2,5,2},
			{0,3,-3},
			{3,4,1},
			{4,5,-4},
			{3,2,2},
	};

	int pointPrecedors[6]={
			0,-1,-1,-1,-1,-1
	};

	bool undefineds[6]={
		false,true,true,true,true,true,
	};

	int pointDistances[6]={
			0,0,0,0,0,0
	};


	int edgesLength = (sizeof(edges)/sizeof(*edges));
	int verticesNumber = 6;

	for(int i=0;i<verticesNumber;i++){
		cout << "Computation for vertice[" << i << "]:" << endl;
		for(int j=0;j<edgesLength;j++){
			int edgeStart = edges[j][0];

			cout << "Checking edges[" << j << "]:" << endl;
			if(edgeStart == i){
				int edgeEnd = edges[j][1];
				int edgeLength = edges[j][2];

				cout << "Got matching edge {" << edgeStart << "," << edgeEnd << "," << edgeLength << "}" << endl;
				if(undefineds[edgeEnd]){
					cout << "First visit of end vertice" << endl;
					pointDistances[edgeEnd] = pointDistances[edgeStart] + edgeLength;
					pointPrecedors[edgeEnd] = edgeStart;
					undefineds[edgeEnd] = false;
				} else {
					cout << "Checking distances" << endl;
					int possibleNewDistance = pointDistances[edgeStart] + edgeLength;

					if(possibleNewDistance < pointDistances[edgeEnd]){
						cout << "Found shorter path!" << endl;
						pointDistances[edgeEnd] = possibleNewDistance;
						pointPrecedors[edgeEnd] = edgeStart;
					}
				}
			}
		}
	}

	cout << "Computation done. Results:" << endl;
	for(int i=0;i<verticesNumber;i++){
		cout << "vertice no." << i << ": dist=" << pointDistances[i] << ", prec=v[" << pointPrecedors[i] << "]" << endl;
	}

	return 0;
}
