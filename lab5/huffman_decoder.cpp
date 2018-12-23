#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct TreeNode {
  TreeNode * left;
  TreeNode * right;
  char ch;
};

void deleteTree(TreeNode * v);
void createTree(TreeNode * & root, ifstream& inputStream);
void decode(TreeNode * root, ifstream& inputStream, ofstream& outputStream);

int main(){
  ifstream inputFile;
	string fileName;
  TreeNode * root;
  string s;

  ofstream outputFile;
  outputFile.open ("decodingOutput.txt");

	inputFile.open ("toDecode.txt", ios::in);
  if (!inputFile.is_open()){
    return 1;
  }

  createTree(root, inputFile);

  decode(root, inputFile, outputFile);

  deleteTree(root);
  outputFile.close();
  inputFile.close();
  return 0;
}


void createTree(TreeNode * & root, ifstream& inputStream){
  int i,j;
  char character;
  string characterCode;
  TreeNode * node;

  root = new TreeNode;
  root->left  = NULL;
  root->right = NULL;

  while(true){
    inputStream >> character >> characterCode;

    if(character == '-' && characterCode == "--"){
      break;
    }

    node = root;

    for(j = 0; j < (int) characterCode.length(); j++){
      if(characterCode[j] == '0'){
        if(!node->left){
          node->left = new TreeNode;
          node->left->left  = NULL;
          node->left->right = NULL;
        }
        node = node->left;
      } else {
        if(!node->right){
          node->right = new TreeNode;
          node->right->left  = NULL;
          node->right->right = NULL;
        }
        node = node->right;
      }

      node->ch = character;
    }
  }
}


void decode(TreeNode * root, ifstream& inputStream, ofstream& outputStream) {
  TreeNode * node;
  string code;
  int i;

  inputStream >> code;

  node = root;

  for(i = 0; i < (int) code.length(); i++){
    if(code[i] == '0'){
      node = node->left;
    } else {
      node = node->right;
    }

    if(!node->left){
      outputStream << node->ch;
      node = root;
    }
  }
}

void deleteTree(TreeNode * node) {
  if(node) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}
