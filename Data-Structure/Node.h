#ifndef GUARD_Node_h
#define GUARD_Node_h

#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<math.h>
#include<stdlib.h>
#include<time.h>
using namespace std;
#define N 10

struct Human;
struct VirusNode {//树节点结构的定义
	VirusNode();//构造函数
	~VirusNode();//析构函数
	bool ifLeaf();//判断是否是叶子结点
	int Incubation;//潜伏期:为整数,以天为单位
	double Infection;//传染性:为实数,每小时感染多少人
	//潜伏期的传染能力为爆发期的1/10
	double DeathRate[3];//致死率:DeathRate[0]表示康复率
	//DeathRate[1]表示持续治疗概率
	//DeathRate[2]表示死亡率
	int VariationTime;//变异所需的时间
	int order;//结点序号
	int stage;//病毒状态,0:潜伏期,1:爆发期,2:隔离救治期
	int timecount;//病毒内置时钟
	int begintime;//病毒初次感染的时间
	bool ifInfect;//标记是否感染过

	int VaccineTime;//疫苗完成制作所需时间
	int VaccineBeginTime;//疫苗开始制作的时间
	bool ifVaccineSucc;//判断疫苗是否研制成功

	vector<Human*> Infected;//正在感染的人
	vector<Human*> Recover;//已经康复的人
	vector<Human*> Death;//已经死亡的人
	VirusNode* subVirus[N];//子结点
	VirusNode* PreVirus;//父亲结点
};

#endif