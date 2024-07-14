#ifndef GUARD_SandBox_h
#define GUARD_SandBox_h

#include"Node.h"
#include"Tree.h"
#include"Human.h"

class SandBox {//沙盒结构:每一种病毒存储了感染该病毒的人数,每一个人存储了其感染的病毒类型
private:
	VirusTree* virus;//该沙箱中的病毒
	vector<Human*> HumanSet;//沙箱人类
	int Population;//人口数
	int IsolationTime;//爆发到隔离所需时间
public:
	SandBox();//构造函数
	~SandBox();//析构函数
	int GetPopulation();//输出此时人口总数
	void SetTree(VirusTree* Tree);//初始化沙箱病毒树
	vector<Human*> GetHumanSet();//初始化人类集
	void Infect(VirusNode* current);//进行感染过程
	void SwitchSituation();//沙盒时间增加
	void Clkdiv();//计数器并且输出
	void SetHuman(VirusTree* Tree);//初始化HumanSet
	void SwicthState(int humanorder);//病人的状态转换
	VirusNode* Variation(VirusNode* current);//病毒变异
	void Break(VirusNode* current);//病毒爆发
	void Isolate(VirusNode* current);//病毒隔离治疗
	void Vaccine(VirusNode* current);//生产疫苗
	void Recover(VirusNode* current, Human* hm);//恢复
	void Kill(VirusNode* current, Human* hm);//死亡
	void UpdateMark();//更新每个人的病毒感染记号
	bool ifSucc();//判断是否成功抵御病毒
	int HourCnt, DayCnt;//时间变量
	VirusTree* GetVirus();//返回病毒树
};

#endif