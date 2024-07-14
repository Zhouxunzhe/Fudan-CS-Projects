#ifndef ast_h
#define ast_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

struct Node {
    string grammar;
    string type;
    string info;
    string error;
    vector<Node*> child;
    int row;
    int col;
};

Node* newgram(string grammar, int row, int col);
Node* newtoken(string type, string info, int row, int col);
void addnode(Node* node1, Node* node2);
void addchild(Node* node1, Node* node2);
void print(Node* node, int level);

void myerror(int tag, string msg, int row, int col);

#endif // !1