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
struct VirusNode {//���ڵ�ṹ�Ķ���
	VirusNode();//���캯��
	~VirusNode();//��������
	bool ifLeaf();//�ж��Ƿ���Ҷ�ӽ��
	int Incubation;//Ǳ����:Ϊ����,����Ϊ��λ
	double Infection;//��Ⱦ��:Ϊʵ��,ÿСʱ��Ⱦ������
	//Ǳ���ڵĴ�Ⱦ����Ϊ�����ڵ�1/10
	double DeathRate[3];//������:DeathRate[0]��ʾ������
	//DeathRate[1]��ʾ�������Ƹ���
	//DeathRate[2]��ʾ������
	int VariationTime;//���������ʱ��
	int order;//������
	int stage;//����״̬,0:Ǳ����,1:������,2:���������
	int timecount;//��������ʱ��
	int begintime;//�������θ�Ⱦ��ʱ��
	bool ifInfect;//����Ƿ��Ⱦ��

	int VaccineTime;//���������������ʱ��
	int VaccineBeginTime;//���翪ʼ������ʱ��
	bool ifVaccineSucc;//�ж������Ƿ����Ƴɹ�

	vector<Human*> Infected;//���ڸ�Ⱦ����
	vector<Human*> Recover;//�Ѿ���������
	vector<Human*> Death;//�Ѿ���������
	VirusNode* subVirus[N];//�ӽ��
	VirusNode* PreVirus;//���׽��
};

#endif