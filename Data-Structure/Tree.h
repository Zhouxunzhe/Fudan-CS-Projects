#ifndef GUARD_Tree_h
#define GUARD_Tree_h

#include"Node.h"
#include"Union.h"

class VirusTree {//���������ඨ��
protected:
	VirusNode* root;
	
private:
	void DestroyTree(VirusNode* current);//����������ɾ��
public:
	VirusTree();//���캯��
	~VirusTree();//��������
	void SaveEveryNode();//���������浽��㼯��
	vector<VirusNode*>NodeSet;//��α����µĽ�㼯��
	vector<VirusNode*>currentNodeSet;//�Ѿ�������ڵĲ������
	VirusNode* GetNearestAncestor
	(Union* Un, VirusNode* root, VirusNode* virus1, VirusNode* virus2);//Ѱ���������Ĺ�������
}; 

#endif