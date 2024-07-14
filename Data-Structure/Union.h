#ifndef GUARD_Union_h
#define GUARD_Union_h

using namespace std;
class VirusTree;
class Union {//并查集的数据结构
private:
	VirusTree *virus;//并查集所包含的树
	vector<int>pre;//前驱结点
	vector<int>height;//height[x]表示以x为根的树的高度
	vector<int>vis;//判断结点是否被遍历
public:
	Union();//构造函数
	~Union();//析构函数
	void Initialize();//并查集初始化
	void SetTree(VirusTree *Tree);//设置并查集当中的树
	int UnionFind(int x);//查找根节点
	bool UnionisConnected(int x, int y);//判断两个结点是否在一个集
	bool UnionJoin(int x, int y);//将两个元素置于同一个集
	void UnionVis(int x);//对遍历过的结点标记
	bool UnionisVis(int x);//判断是否标记
	VirusTree* GetVirus();//返回病毒树
};

#endif