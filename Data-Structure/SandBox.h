#ifndef GUARD_SandBox_h
#define GUARD_SandBox_h

#include"Node.h"
#include"Tree.h"
#include"Human.h"

class SandBox {//ɳ�нṹ:ÿһ�ֲ����洢�˸�Ⱦ�ò���������,ÿһ���˴洢�����Ⱦ�Ĳ�������
private:
	VirusTree* virus;//��ɳ���еĲ���
	vector<Human*> HumanSet;//ɳ������
	int Population;//�˿���
	int IsolationTime;//��������������ʱ��
public:
	SandBox();//���캯��
	~SandBox();//��������
	int GetPopulation();//�����ʱ�˿�����
	void SetTree(VirusTree* Tree);//��ʼ��ɳ�䲡����
	vector<Human*> GetHumanSet();//��ʼ�����༯
	void Infect(VirusNode* current);//���и�Ⱦ����
	void SwitchSituation();//ɳ��ʱ������
	void Clkdiv();//�������������
	void SetHuman(VirusTree* Tree);//��ʼ��HumanSet
	void SwicthState(int humanorder);//���˵�״̬ת��
	VirusNode* Variation(VirusNode* current);//��������
	void Break(VirusNode* current);//��������
	void Isolate(VirusNode* current);//������������
	void Vaccine(VirusNode* current);//��������
	void Recover(VirusNode* current, Human* hm);//�ָ�
	void Kill(VirusNode* current, Human* hm);//����
	void UpdateMark();//����ÿ���˵Ĳ�����Ⱦ�Ǻ�
	bool ifSucc();//�ж��Ƿ�ɹ���������
	int HourCnt, DayCnt;//ʱ�����
	VirusTree* GetVirus();//���ز�����
};

#endif