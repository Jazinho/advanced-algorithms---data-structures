#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

void bfs_L(vector<vector<int> > edges, queue<int> &path, int searchedVertice, int verticesNumber);
void bfs_M(vector<vector<int> > matrix, queue<int> &path, int searchedVertice, int verticesNumber);

int main() {
	string line;
	vector<vector<int> > edgesVector;
	int verticesNumber;
	ifstream dataFile;
	string fileName;
	string method;
	const int MAXINT = 2147483647;

	cout << "Wpisz 'L' aby wczytać dane z pliku z listą krawędzi." << endl;
	cout << "Wpisz 'M' aby wczytać dane z pliku z macierzą sąsiedztwa." << endl;
	cin >> method;

	if(method == "L"){
		dataFile.open ("edges_data.csv");
	} else if(method == "M"){
		dataFile.open ("matrix_data.csv");
	} else {
		cout << "Wrong option" << endl;
		return 1;
	}

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

		dataFile.close();
	} else {
		cout << "Cannot open file!";
		return 1;
	}

	int edgesLength = edgesVector.size();

	vector<int> channelFlows (edgesLength, 0);
	vector<int> channelRest (edgesLength, 0);
	queue<int> path;

	if(method == "L"){
		bfs_L(edgesVector, path, verticesNumber-1, verticesNumber);
	} else {
		bfs_M(edgesVector, path, verticesNumber-1, verticesNumber);
	}


	return 0;
}


void bfs_L(vector<vector<int> > edges, queue<int> &path, int searchedVertice, int verticesNumber){
	queue<int> q;
	vector<bool> visited(verticesNumber, false);
	vector<int> precedors(verticesNumber, -1);
	q.push(0);
	precedors[0]=0;
	visited[0]=true;

	while(q.size() != 0){
		int curr = q.front();
		q.pop();

		for(int i=0; i<edges.size();i++){
			int destVert = edges[i][1];
			if(edges[i][0]==curr && !visited[destVert]){
				precedors[destVert]=curr;
				if(destVert==searchedVertice){
					cout << "BFS -> Shortest path (end <--- start): \n";
					path.push(searchedVertice);
					cout << searchedVertice << " ";
					curr = searchedVertice;
					int tmp;

					while(curr != 0){
						tmp = curr; 
						curr = precedors[curr];
						path.push(curr);
						cout << curr << " ";
					}
					
					cout << "\n";
					return;
				}
				
				q.push(destVert);
				visited[destVert]=true;
			}
		}
	}
}


void bfs_M(vector<vector<int> > matrix, queue<int> &path, int searchedVertice, int verticesNumber){
	queue<int> q;
	vector<bool> visited(verticesNumber, false);
	vector<int> precedors(verticesNumber, -1);
	q.push(0);
	precedors[0]=0;
	visited[0]=true;

	while(q.size() != 0){
		int curr = q.front();
		q.pop();

		for(int i=0;i<verticesNumber;i++){
            for(int destVert=0;destVert<verticesNumber;destVert++){
			    if(i==curr && matrix[i][destVert]!=0 && !visited[destVert]){
				    precedors[destVert]=curr;
				    if(destVert==searchedVertice){
					    cout << "BFS -> Shortest path (end <--- start): \n";
					    path.push(searchedVertice);
					    cout << searchedVertice << " ";
					    curr = searchedVertice;
					    int tmp;

					    while(curr != 0){
						    tmp = curr; 
						    curr = precedors[curr];
						    path.push(curr);
						    cout << curr << " ";
					    }
					
					    cout << "\n";
					    return;
				    }
				
				    q.push(destVert);
				    visited[destVert]=true;
			    }
            }
		}
	}
}



