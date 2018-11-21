#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;

int getMaxFlowForAnotherPath_L(vector<vector<int> > edges, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> > &flows, vector<int> &potentialFlows);
int getMaxFlowForAnotherPath_M(vector<vector<int> > matrix, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> > &flows, vector<int> &potentialFlows);

int main() {
	string line;
	vector<vector<int> > edgesVector;
	vector<vector<int> > flows;
    vector<vector<int> > reversedFlows;
	int verticesNumber, max;
    int totalFlow = 0;
	ifstream dataFile;
	string fileName;
	string method;
	const int MAXINT = 214748364;

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
    vector<int> potentialFlow(verticesNumber);
	deque<int> path;
    bool flag = true;
    flows = edgesVector;

    if(method == "L"){
	    for(int i=0;i<edgesLength;i++){
            flows[i][2] = 0;
            flows[i].push_back(0);
        }
    } else {
    	for(int i=0;i<verticesNumber;i++){
            for(int j=0;j<verticesNumber;j++){
                flows[i][j] = 0;
            }
            potentialFlow[i]=0;
        }
    }

    while(true){
        deque<int> path;
        potentialFlow[0]=1000000;
	    if(method == "L"){
		    max = getMaxFlowForAnotherPath_L(edgesVector, path, verticesNumber-1, verticesNumber, flows, potentialFlow);
	    } else {
		    max = getMaxFlowForAnotherPath_M(edgesVector, path, verticesNumber-1, verticesNumber, flows, potentialFlow);
	    }

        if(path.size() == 0){
           cout << "Another path not found...\n";
           break;
        }

        totalFlow += max;
    }

    cout << "Flows function:\n";

    /*if(method == "L"){
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
    }*/

    cout << "Maximum flow is:" << totalFlow << "\n";

	return 0;
}


int getMaxFlowForAnotherPath_L(vector<vector<int> > edges, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> > &flows, vector<int> &potentialFlows){
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
			int startVert = edges[i][0];
			int destVert = edges[i][1];
            int residualFlow = edges[i][2]-flows[i][2]; //what more can i put into pipe (capacity minus currentlyFlowing)
                                                            // przepływ rezydualny - potencjał w przepływie
            //int reverseResidualFlow = edges[i][2]-flows[i][3];

            //cout << "reee " << residualFlow << "\n";
			if(startVert==curr && !visited[destVert] && residualFlow > 0){
				precedors[destVert]=curr;
                potentialFlows[destVert] = potentialFlows[startVert] > residualFlow ? residualFlow : potentialFlows[startVert];

				if(destVert==searchedVertice){
                    int maxFlow = potentialFlows[destVert];
					cout << "BFS -> Shortest path (end <--- start): \n";
					path.push_back(searchedVertice);
					cout << searchedVertice << " ";
					curr = searchedVertice;
					int end;

					while(curr != 0){
						end = curr; 
						curr = precedors[end];
						path.push_back(curr);
						cout << curr << " ";

                        for(int j=0;j<flows.size();j++){
                            if(flows[j][0]==curr && flows[j][1]==end){
                                int currentFlow = flows[j][2];
                                //int currentReverseFlow = reversedFlows[j][2];
                                flows[j][2]=currentFlow + maxFlow;
                                //reversedFlows[j][2]=currentReverseFlow - maxFlow;
                            }
                        }
					}
					
					cout << "\n";
					return maxFlow;
				}
				
				q.push_back(destVert);
				visited[destVert]=true;
			}
		}
	}
}


int getMaxFlowForAnotherPath_M(vector<vector<int> > matrix, deque<int> &path, int searchedVertice, int verticesNumber, vector<vector<int> > &flows, vector<int> &potentialFlows){
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
                int residualFlow = matrix[i][destVert]-flows[i][destVert]; //what more can i put into pipe (capacity minus currentlyFlowing)
                                                                           // przepływ rezydualny - potencjał w przepływie

			    if(i==curr && matrix[i][destVert]!=0 && !visited[destVert] && residualFlow > 0){
				    precedors[destVert]=curr;
                    potentialFlows[destVert] = potentialFlows[curr] > residualFlow ? residualFlow : potentialFlows[curr];

				    if(destVert==searchedVertice){
                        int maxFlow = potentialFlows[destVert];
					    cout << "BFS -> Shortest path (end <--- start): \n";
					    path.push_back(searchedVertice);
					    cout << searchedVertice << " ";
					    curr = searchedVertice;
					    int endVert;

					    while(curr != 0){
						    endVert = curr; 
						    curr = precedors[endVert];
						    path.push_back(curr);
						    cout << curr << " ";

                            for(int start=0;start<flows.size();start++){
                                for(int end=0;end<flows.size();end++){
                                    if(start==curr && end==endVert){
                                        int currentFlow = flows[start][end];
                                        int currentReverseFlow = flows[end][start];
                                        flows[start][end]=currentFlow + maxFlow;
                                        flows[end][start]=currentReverseFlow - maxFlow;
                                    }
                                }
                            }
					    }
					
					    cout << "\n";
					    return maxFlow;
				    }
				
				    q.push_back(destVert);
				    visited[destVert]=true;
			    }
            }
		}
	}
}



