#ifndef GUARD_Tree_h
#define GUARD_Tree_h

#include"Node.h"
#include"Union.h"

class VirusTree {//病毒树的类定义
protected:
	VirusNode* root;
	
private:
	void DestroyTree(VirusNode* current);//将结点的子树删除
public:
	VirusTree();//构造函数
	~VirusTree();//析构函数
	void SaveEveryNode();//将病毒结点存到结点集中
	vector<VirusNode*>NodeSet;//层次遍历下的结点集合
	vector<VirusNode*>currentNodeSet;//已经变异存在的病毒结点
	VirusNode* GetNearestAncestor
	(Union* Un, VirusNode* root, VirusNode* virus1, VirusNode* virus2);//寻找两个结点的公共祖先
}; 

#endif