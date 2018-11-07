#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;

void bfs_L(vector<vector<int> > edges, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> >flows);
void bfs_M(vector<vector<int> > matrix, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> >flows);
int find_min_L(vector<vector<int> > edgesVector, deque<int> &path, vector<vector<int> >flows);
int find_min_M(vector<vector<int> > edgesVector, deque<int> &path, vector<vector<int> >flows);
void calculate_flows_L(int min, deque<int> &path, vector<vector<int> > &flows, vector<vector<int> > &reversedFlows);
void calculate_flows_M(int min, deque<int> &path, vector<vector<int> > &flows, vector<vector<int> > &reversedFlows);

int main() {
	string line;
	vector<vector<int> > edgesVector;
	vector<vector<int> > flows;
    vector<vector<int> > reversedFlows;
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

	deque<int> path;
    bool flag = true;
    flows = edgesVector;
    reversedFlows = edgesVector;

    if(method == "L"){
	    for(int i=0;i<edgesLength;i++){
            flows[i][2] = 0;
            reversedFlows[i][2] = 0;
        }
    } else {
	    for(int i=0;i<verticesNumber;i++){
            for(int j=0;j<verticesNumber;j++){
                flows[i][j] = 0;
                reversedFlows[i][j] = 0;
            }
        }
    }

    while(true){
        deque<int> path;
	    if(method == "L"){
		    bfs_L(edgesVector, path, verticesNumber-1, verticesNumber, flows);
	    } else {
		    bfs_M(edgesVector, path, verticesNumber-1, verticesNumber, flows);
	    }

        if(path.size() == 0){
           cout << "Another path not found...\n";
           break;
        }

        int min;
        if(method == "L"){
		    min = find_min_L(edgesVector, path, flows);
	    } else {
		    min = find_min_M(edgesVector, path, flows);
	    }

        cout << "Minimal flow for path: " << min << "\n\n";
        
        if(method == "L"){
		    calculate_flows_L(min, path, flows, reversedFlows);
	    } else {
		    calculate_flows_M(min, path, flows, reversedFlows);
	    }
    }

    cout << "Flows function:\n";

    if(method == "L"){
        for(int i=0;i<edgesLength;i++){
            cout << flows[i][0] << "--> (" << flows[i][2] << ") -->" << flows[i][1] << "\n";
        }
    } else {
        for(int i=0;i<verticesNumber;i++){
            for(int j=0;j<verticesNumber;j++){
                cout << flows[i][j] << " ";
            }

            cout << "\n";
        }
    }


	return 0;
}


void bfs_L(vector<vector<int> > edges, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> >flows){
	deque<int> q;
	vector<bool> visited(verticesNumber, false);
	vector<int> precedors(verticesNumber, -1);
	q.push_back(0);
	precedors[0]=0;
	visited[0]=true;

	while(q.size() != 0){
		int curr = q.front();
		q.pop_front();

		for(int i=0; i<edges.size();i++){
			int destVert = edges[i][1];
			if(edges[i][0]==curr && !visited[destVert] && edges[i][2]-flows[i][2] > 0){
				precedors[destVert]=curr;
				if(destVert==searchedVertice){
					cout << "BFS -> Shortest path (end <--- start): \n";
					path.push_back(searchedVertice);
					cout << searchedVertice << " ";
					curr = searchedVertice;
					int tmp;

					while(curr != 0){
						tmp = curr; 
						curr = precedors[curr];
						path.push_back(curr);
						cout << curr << " ";
					}
					
					cout << "\n";
					return;
				}
				
				q.push_back(destVert);
				visited[destVert]=true;
			}
		}
	}
}


void bfs_M(vector<vector<int> > matrix, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> >flows){
	deque<int> q;
	vector<bool> visited(verticesNumber, false);
	vector<int> precedors(verticesNumber, -1);
	q.push_back(0);
	precedors[0]=0;
	visited[0]=true;

	while(q.size() != 0){
		int curr = q.front();
		q.pop_front();

		for(int i=0;i<verticesNumber;i++){
            for(int destVert=0;destVert<verticesNumber;destVert++){
			    if(i==curr && matrix[i][destVert]!=0 && !visited[destVert] && matrix[i][destVert]-flows[i][destVert] > 0){
				    precedors[destVert]=curr;
				    if(destVert==searchedVertice){
					    cout << "BFS -> Shortest path (end <--- start): \n";
					    path.push_back(searchedVertice);
					    cout << searchedVertice << " ";
					    curr = searchedVertice;
					    int tmp;

					    while(curr != 0){
						    tmp = curr; 
						    curr = precedors[curr];
						    path.push_back(curr);
						    cout << curr << " ";
					    }
					
					    cout << "\n";
					    return;
				    }
				
				    q.push_back(destVert);
				    visited[destVert]=true;
			    }
            }
		}
	}
}


int find_min_L(vector<vector<int> > edgesVector, deque<int> &path, vector<vector<int> >flows){
    int min = 10000000;    
    for(int i=path.size()-1;i>0;i--){
        int edgeStart = path[i];
        int edgeEnd = path[i-1];

        for(int j=0;j<edgesVector.size();j++){
            int possibleMinFlow = edgesVector[j][2]-flows[j][2];

            if(edgesVector[j][0]==edgeStart && edgesVector[j][1]==edgeEnd && possibleMinFlow < min){
                min = possibleMinFlow;
            }
        }
    }

    return min;
}


int find_min_M(vector<vector<int> > edgesVector, deque<int> &path, vector<vector<int> >flows){
    int min = 10000000;    
    for(int i=path.size()-1;i>0;i--){
        int edgeStart = path[i];
        int edgeEnd = path[i-1];

        for(int start=0;start<edgesVector.size();start++){
            for(int end=0;end<edgesVector.size();end++){
                int possibleMinFlow = edgesVector[start][end]-flows[start][end];

                if(start==edgeStart && end==edgeEnd && possibleMinFlow < min){
                    min = possibleMinFlow;
                }
            }
        }
    }

    return min;
}


void calculate_flows_L(int min, deque<int> &path, vector<vector<int> > &flows, vector<vector<int> > &reversedFlows){
    for(int i=path.size()-1;i>0;i--){
        int edgeStart = path[i];
        int edgeEnd = path[i-1];

        for(int j=0;j<flows.size();j++){
            if(flows[j][0]==edgeStart && flows[j][1]==edgeEnd){
                int currentFlow = flows[j][2];
                int currentReverseFlow = reversedFlows[j][2];
                flows[j][2]=currentFlow + min;
                reversedFlows[j][2]=currentReverseFlow - min;
            }
        }
    }
}


void calculate_flows_M(int min, deque<int> &path, vector<vector<int> > &flows, vector<vector<int> > &reversedFlows){
    for(int i=path.size()-1;i>0;i--){
        int edgeStart = path[i];
        int edgeEnd = path[i-1];

        for(int start=0;start<flows.size();start++){
            for(int end=0;end<flows.size();end++){
                if(start==edgeStart && end==edgeEnd){
                    int currentFlow = flows[start][end];
                    int currentReverseFlow = reversedFlows[start][end];
                    flows[start][end]=currentFlow + min;
                    reversedFlows[start][end]=currentReverseFlow - min;
                }
            }
        }
    }
}



