#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

struct TreeNode
{
  TreeNode * next;
  TreeNode * left;
  TreeNode * right;
  char ch;
  int count;
};

struct Cmp
{
    bool operator ()(const pair<char, int> &a, const pair<char, int> &b)
    {
        return a.second > b.second;
    }
} comp;

int main() {
	string line;
	ifstream dataFile;
	string fileName;
	string method;
	map<char, int> codeMap;
	map<char, int>::iterator it;
	char character;
	string code;

	cout << "Wpisz 'e' aby zakodować dane." << endl;
	cout << "Wpisz 'd' aby rozkodować." << endl;
	cin >> method;

	if(method == "e"){
		dataFile.open ("toEncode.txt", ios::in);

		while (!dataFile.eof() ) {
			dataFile.get(character);
			if(character != ' ' && character != '\n'){
				if(codeMap.find(character) == codeMap.end()){
					codeMap.insert(make_pair(character, 1));
				} else {
					it = codeMap.find(character);
					int num = it->second;
					it->second = num+1;
				}
			}
		}

	} else if(method == "d"){
		dataFile.open ("toDecode.txt");
		if (dataFile.is_open()){

			while ( getline (dataFile,line) ){
				if(line == ""){
					break;
				}

				stringstream ss(line);
				char i;
				int occurences;
				ss >> i;
				ss.ignore();
				ss >> occurences;
				codeMap.insert(make_pair(i,occurences));
	    }

			getline (dataFile,line);
			code = line;
			dataFile.close();
		} else {
			cout << "Cannot open file!";
			return 1;
		}
	} else {
		cout << "Wrong option" << endl;
		return 1;
	}

	if(method == "e"){

	} else {
		cout << code << "\n";
	}

	for (it = codeMap.begin(); it != codeMap.end(); ++it ){
		cout << "[" << it->first << "]: " << it->second << "\n";
	}

	cout << "\n";
	vector<pair<char,int> > codeVect(codeMap.begin(), codeMap.end());
	vector<pair<char,int> > codeTree(codeVect.size()*(codeVect.size()-1)/2);

	// for(int i=0;i<codeVect.size();i++){
	// 	if(i>=)
	// 	codeTree.push_back
	// }

	sort(codeVect.begin(), codeVect.end(), comp);

	// while(codeVect.size() > 1){
	// 	int last = codeVect.pop_back();
	// 	int befLast = codeVect.pop_back();
	// 	codeVect.push_back()
	// }

	for (int i=0;i< codeVect.size();i++){
		cout << "[" << codeVect[i].first << "]: " << codeVect[i].second << "\n";
	}

	return 0;
}
