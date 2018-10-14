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
	int verticesNumber;

	if (dataFile.is_open()){
		getline (dataFile,line);
		stringstream ss(line);
		ss >> verticesNumber;

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

		// for (int k=0; k< edgesVector.size(); k++){
		// 	for (int l=0; l< edgesVector.at(k).size(); l++){
    //     std::cout << edgesVector.at(k).at(l);
		// 	}
		// 	cout << endl;
		// }

		dataFile.close();
	} else {
		cout << "Cannot open file!";
		return 1;
	}

	vector<int> pointPrecedors (verticesNumber, -1);
	pointPrecedors[0] = 0;

	vector<bool> undefineds(verticesNumber, true);
	undefineds[0] = false;

	vector<int> pointDistances (verticesNumber, 0);

	int edgesLength = edgesVector.size();

	for(int i=0;i<verticesNumber;i++){
		// cout << "Computation for vertice[" << i << "]:" << endl;
		for(int j=0;j<edgesLength;j++){
			int edgeStart = edgesVector.at(j).at(0);

			// cout << "Checking edges[" << j << "]:" << endl;
			if(edgeStart == i){
				int edgeEnd = edgesVector.at(j).at(1);
				int edgeLength = edgesVector.at(j).at(2);

				// cout << "Got matching edge {" << edgeStart << "," << edgeEnd << "," << edgeLength << "}" << endl;
				if(undefineds[edgeEnd]){
					// cout << "First visit of end vertice" << endl;
					pointDistances[edgeEnd] = pointDistances[edgeStart] + edgeLength;
					pointPrecedors[edgeEnd] = edgeStart;
					undefineds[edgeEnd] = false;
				} else {
					// cout << "Checking distances" << endl;
					int possibleNewDistance = pointDistances[edgeStart] + edgeLength;

					if(possibleNewDistance < pointDistances[edgeEnd]){
						// cout << "Found shorter path!" << endl;
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
