#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct TreeNode
{
  TreeNode * next;
  TreeNode * left;
  TreeNode * right;
  char ch;
  int count;
};

void createList(TreeNode * & root, string s);
void listToTree(TreeNode * & root);
void treeToFile(TreeNode * p, string b, ofstream& outputFile);
bool encodeChar(char c, TreeNode * p, string b, ofstream& outputFile);
void encode(TreeNode * root, string s, ofstream& outputFile);
void deleteTree(TreeNode * v);

int main(){
  ifstream dataFile;
	string fileName;
  TreeNode * root;
  string s;

  ofstream outputFile;
  outputFile.open ("encodingOutput.txt");

	dataFile.open ("toEncode.txt", ios::in);
  if (dataFile.is_open()){
		getline (dataFile,s);
    dataFile.close();
  } else {
    return 1;
  }

  createList(root,s);

  listToTree(root);

  treeToFile(root,"", outputFile);
  outputFile << endl;

  encode(root,s, outputFile);
  outputFile << endl;

  deleteTree(root);
  outputFile.close();
  return 0;
}


void createList(TreeNode * & root, string s) {
  int i,tmp_count;
  char tmp_char;
  TreeNode * p;
  bool isSorted;

  root = NULL;
  for(i = 0; i < s.length(); i++){
    p = root;
    while (p && (p->ch != s[i])){ //sprawdzamy czy napotkany znak wystąpił już wcześniej
      p = p->next;
    }

    if(!p) {
      p = new TreeNode;
      p->next  = root;
      p->left  = NULL;
      p->right = NULL;
      p->ch    = s[i];
      p->count = 1;
      root     = p;
    } else {
      p->count++;
    }
  }

  do {
    isSorted = true;
    p = root;
    while(p->next) {
      if(p->count > p->next->count) {
        tmp_char = p->ch;
        p->ch = p->next->ch;
        p->next->ch = tmp_char;

        tmp_count = p->count;
        p->count = p->next->count;
        p->next->count = tmp_count;
        isSorted = false;
      }
      p = p->next;
    }
  } while(!isSorted);
}


void listToTree(TreeNode * & root){
  TreeNode *newNode, *nodeIterator, *first, *second;

  while(true) {
    first = root;
    second = root->next;

    if(!second) break;

    root = second->next; //początek listy ustawiamy na trzeci element

    newNode = new TreeNode;
    newNode->left = first;
    newNode->right = second;
    newNode->count = first->count + second->count;

     //jak nie ma więcej elementów na liście, albo newNode ma mniejszy count niż pierwszy element na pozostałej liście, to wstaw newNode na początek
    if(!root || (newNode->count <= root->count)) {
      newNode->next = root;
      root = newNode;
      continue;
    }

    nodeIterator = root;

    while (nodeIterator->next && (newNode->count > nodeIterator->next->count)){
      nodeIterator = nodeIterator->next;
    }

    //doszliśmy do końca lub znaleźliśmy większy count niż newNode.count
    //nodeIterator <= newNode <= nodeIterator->next
    newNode->next = nodeIterator->next;
    nodeIterator->next = newNode;
  }
}


void treeToFile(TreeNode * p, string b, ofstream& output){
  if(!p->left){
    output << p->ch << " " << b << endl;
  } else {
    treeToFile(p->left, b + "0", output);
    treeToFile(p->right,b + "1", output);
  }
}


bool encodeChar(char c, TreeNode * p, string b, ofstream& output){
  if(!p->left) {
    if(c != p->ch) return false;
    else {
      output << b;
      return true;
    }
  } else
    return encodeChar(c, p->left, b+"0", output) || encodeChar(c, p->right, b+"1", output);
}


void encode(TreeNode * root, string s, ofstream& output){
  for(int i = 0; i < s.length(); i++){
    encodeChar(s[i],root,"", output);
  }
}


void deleteTree(TreeNode * v) {
  if(v) {
    deleteTree(v->left);
    deleteTree(v->right);
    delete v;
  }
}
