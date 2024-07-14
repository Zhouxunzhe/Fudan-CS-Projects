#ifndef GUARD_Union_h
#define GUARD_Union_h

using namespace std;
class VirusTree;
class Union {//���鼯�����ݽṹ
private:
	VirusTree *virus;//���鼯����������
	vector<int>pre;//ǰ�����
	vector<int>height;//height[x]��ʾ��xΪ�������ĸ߶�
	vector<int>vis;//�жϽ���Ƿ񱻱���
public:
	Union();//���캯��
	~Union();//��������
	void Initialize();//���鼯��ʼ��
	void SetTree(VirusTree *Tree);//���ò��鼯���е���
	int UnionFind(int x);//���Ҹ��ڵ�
	bool UnionisConnected(int x, int y);//�ж���������Ƿ���һ����
	bool UnionJoin(int x, int y);//������Ԫ������ͬһ����
	void UnionVis(int x);//�Ա������Ľ����
	bool UnionisVis(int x);//�ж��Ƿ���
	VirusTree* GetVirus();//���ز�����
};

#endif