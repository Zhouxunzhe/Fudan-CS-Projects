#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "ast.h"
using namespace std;

Node* newgram(string grammar, int row, int col){
  Node* node = new Node ();
  node->grammar = grammar;
  node->row = row;
  node->col = col;
  return node;
}
Node* newtoken(string type, string info, int row, int col){
  Node* node = new Node();
  node->type = type;
  node->info = info;
  node->row = row;
  node->col = col;
  return node;
}
bool isnull(Node* node){
  if(node->grammar == "" && node->type == "")
    return true;
  return false;
}
void addnode(Node* node1, Node* node2){
  if( !isnull(node1) && !isnull(node2) )
    node1->child.push_back(node2);
}
void addchild(Node* node1, Node* node2){
  if( !isnull(node1) && !isnull(node2) ){
    for(int i=0;i<node2->child.size();i++){
      node1->child.push_back(node2->child[i]);
    }
  }
}
void print(Node* node, int level){
  for(int i = 0;i<level;i++){
    cout<<"..";
  }
  if(node->type!=""){
    cout<<" <"<<node->row <<","<<node->col<<"> "<<"Token: "<<node->type<<": "<<node->info<<endl;
  }
  else if(node->error!=""){
    cout<<" <"<<node->row <<","<<node->col<<"> "<<"Error: "<<node->grammar<<endl;
  }
  else if(node->grammar!=""){
    cout<<" <"<<node->row <<","<<node->col<<"> "<<"Grammar: "<<node->grammar<<" "<<node->info<<endl;
  }
  else cout<<"null"<<endl;
  for(int i=0;i<node->child.size();i++){
    int tmp = level+1;
    print(node->child[i], tmp);
  }
}
void myerror(int tag, string msg, int row, int col) {
  if(tag){ 
    string str = "syntax error: <" + to_string(row) + ", " + to_string(col) + "> " + msg;
    const char* errmsg = str.c_str();
    cout << errmsg << endl;
  }
  else{
    string str = "invalid token: <" + to_string(row) + ", " + to_string(col) + "> " + msg;
    const char* errmsg = str.c_str();
    cout << errmsg << endl;
  }
}

int yyparse();
extern "C" FILE* yyin;
extern Node* root;

int main(int argc, char* argv[]) {
  if (argc > 2) {
    string out(argv[2]);
    string str(argv[1]);
    string op = "/home/zhouxunzhe/bison-3.8.1/z-COMPILE/project2/results/case_" + str + ".txt";
    const char* outpath = op.c_str();
    if(strcmp(argv[2], "-f") == 0){
      ofstream fout(outpath);
      streambuf* tostd = cout.rdbuf(fout.rdbuf());
      string ip = "/home/zhouxunzhe/bison-3.8.1/z-COMPILE/project2/tests/case_" + str + ".pcat";
      const char* inpath = ip.c_str();
      FILE *file = fopen(inpath, "r");
      if (!file) {
          cerr << "cannot open file." << endl;
          return 1;
      } else {
          yyin = file;
      }
      int n = yyparse();
      if( !n && strcmp(argv[1], "11") != 0 ){ // read EOF
        print(root, 0);
      }
    }
    else if(strcmp(argv[2], "-s") != 0){
      cout << "usage: ./main filenum -f/-s" << endl;
      return 1;
    }
    else {
      string ip = "/home/zhouxunzhe/bison-3.8.1/z-COMPILE/project2/tests/case_" + str + ".pcat";
      const char* inpath = ip.c_str();
      FILE *file = fopen(inpath, "r");
      if (!file) {
          cerr << "cannot open file." << endl;
          return 1;
      } else {
          yyin = file;
      }
      int n = yyparse();
      if( !n && strcmp(argv[1], "11") != 0 ){ // read EOF
        print(root, 0);
      }
    }
  } 
  else {
    cout << "usage: ./main filenum -f/-s" << endl;
    cout << "you can input down here." << endl;
    yyin = stdin;
    int n = yyparse();
    if( !n && strcmp(argv[1], "11") != 0 ){ // read EOF
      print(root, 0);
    }
  }
  return 0;
}