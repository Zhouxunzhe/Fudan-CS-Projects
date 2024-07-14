#include"Human.h"
#include"Node.h"
#include"Tree.h"
#include"Union.h"
#include"SandBox.h"
using namespace std;
#define N 10//������
int MAXDAY;//Ǳ����,����ʱ��,��������ʱ�������������
int POPULATION;//ɳ���˿�����

Human::~Human() {
	InfectedVirus.clear();
}
Human::Human() {
	virus = NULL;
}
void Human::Initialize(VirusTree* Tree) {//��������ʼ��
	virus = Tree;
}

VirusNode::VirusNode() {//ÿ�������г�ʼ��
	Incubation = rand() % MAXDAY;//Ǳ���ڵ��趨��ΧΪ0~MAXDAY�������
	Infection = rand() / (double)(RAND_MAX) * POPULATION;
	//��Ⱦ�ʵ��趨��ΧΪ0~POPULATION��double���͵������
	double a = rand() % 100, b = rand() % 100, c = rand() % 100;
	double total = a + b + c;
	DeathRate[0] = a / total; DeathRate[1] = b / total; DeathRate[2] = c / total;
	//�����ʵ��趨�������,���߸����ܺͼ�����Ϊ1
	//DeathRate[0]��ʾ������
	//DeathRate[2]��ʾ�������Ƹ���
	//DeathRate[3]��ʾ������
	VariationTime = rand() % MAXDAY;//���������ʱ�䷶ΧΪ0~MAXDAY�������
	order = 0;
	Infected.clear(); Recover.clear(); Death.clear();
	stage = 0;
	timecount = 0;
	begintime = 0;
	VaccineBeginTime = 0;
	VaccineTime = rand() % MAXDAY;//������������ʱ�䷶ΧΪ0~MAXDAY�������
	ifInfect = false;
	ifVaccineSucc = false;
	for (int i = 0; i < N; i++) {
		subVirus[i] = NULL;
	}
	PreVirus = NULL;
}
VirusNode::~VirusNode() {
	Infected.clear();
	delete[] subVirus;
	delete PreVirus;
}
bool VirusNode::ifLeaf() {//��������ӽ��ΪNULL��ýڵ�ΪҶ�ӽ��
	for (int i = 0; i < N; i++) {
		if (subVirus[i] != NULL) {
			return false;
		}
	}
	return true;
}

double GaussRand(int Infection) {//��˹����,�õ���̬�ֲ��������
	static double V1, V2, S;
	static int phase = 0;
	double x;
	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);
		x = V1 * sqrt(-2 * log(S) / S);
	}
	else
		x = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	return (x * 0.5 + Infection);//����Ϊ��Ⱦ�ʣ���׼��Ϊ100
}
SandBox::SandBox() {
	virus = new VirusTree;
	HumanSet.clear();
	Population = POPULATION;//��������
	IsolationTime = rand() % MAXDAY;//��������������ʱ���������
	HourCnt = 0; DayCnt = 0;//ɳ������ʱ��
}
SandBox::~SandBox() {
	virus->~VirusTree();
	HumanSet.clear();
}
int SandBox::GetPopulation() {
	Population = HumanSet.size();
	return Population;
}
void SandBox::SetTree(VirusTree* Tree) {
	virus = Tree;
}
vector<Human*> SandBox::GetHumanSet() {
	return HumanSet;
}
bool SandBox::ifSucc(){//ͨ�������ܷ������Ⱦ�ж������Ƿ�ʤ��
	for (int i = 0; i < virus->currentNodeSet.size(); i++) {
		if (virus->currentNodeSet[i]->Infection != 0)
			return false;
	}
	if (HumanSet.size() > 0)
		return true;
	else
		return false;
}
void SandBox::Vaccine(VirusNode* current) {//������������Ʋ����ò�����������״̬
	if ((current->timecount + current->begintime) /24
		>= (current->VaccineTime + current->VaccineBeginTime / 24)) {
		current->ifVaccineSucc = true;//�����������
		current->Infection = 0;//��������Ⱦ������Ϊ0,�����ܼ�����Ⱦ
		if (current->VariationTime != 0)
			current->VariationTime = rand() % current->VariationTime + current->VariationTime + 1;
		//�������Ƴɹ������ʱ�佫�������ΪVariationTime~2*VariationTime����
		for (int i = 0; i < current->Infected.size(); i++) {//ÿһ�����˿���
			Recover(current, current->Infected[i]);
		}
	}
}
void SandBox::Infect(VirusNode* current) {//���в����Ĵ�������
	if (current->ifInfect == false) {
		current->ifInfect = true;//���ò�����Ⱦ״̬
		current->begintime = HourCnt;//��ʼ����������ʱ��
		current->VaccineBeginTime = rand() % current->begintime;
		//��������ٶ��ڸմ�Ⱦ��֮ǰ�Ϳ�ʼ����
	}
	if (current->ifVaccineSucc == false) {
		int intInf = (int)GaussRand(current->Infection);
		//���ø�Ⱦ����ΪΧ������ֵ(��Ⱦ��)��̫�ֲ�����ֵ
		if(intInf<=HumanSet.size()){//��������������Ⱦ(�����ظ���Ⱦ)
			for (int i = 0; i < intInf; i++) {
				int random = rand() % HumanSet.size();//���������,�Բ�������Ⱦ��ͳ��ֵ���������д�Ⱦ
				if (HumanSet[random]->mark[current->order] != 1) {//�ж��Ƿ��Ѿ���Ⱦ���ò���
					HumanSet[random]->InfectedVirus.push_back(current);
					HumanSet[random]->mark[current->order] = 1;
					current->Infected.push_back(HumanSet[random]);
				}
			}
		}
		else//�����Ⱦ�ʸ��ڴ�������,���ÿһ�������˽��д�Ⱦ
			for (int i = 0; i < HumanSet.size(); i++) {
				int random = rand() % HumanSet.size();
				if (HumanSet[random]->mark[current->order] != 1) {
					HumanSet[random]->InfectedVirus.push_back(current);
					HumanSet[random]->mark[current->order] = 1;
					current->Infected.push_back(HumanSet[random]);
				}
			}
	}
}
void SandBox::SetHuman(VirusTree* Tree) {//�����ܵ����༯��
	for (int i = 0; i < Population; i++) {
		Human* hm = new Human;
		hm->Initialize(Tree);//��ÿһ����������������ĳ�ʼ��
		HumanSet.push_back(hm);
	}
}
void SandBox::Clkdiv() {//ʱ������,��������ı������ÿ���ɳ��ģ�����
	HourCnt++;//ʱ�뿪ʼ����
	if (HourCnt % 24 == 0) {
		DayCnt++;//���ʱ��ת��24Сʱ,��������һλ
		cout << "Day" << DayCnt << ": " << endl;
		for (int i = 0; i < virus->currentNodeSet.size(); i++) {
			cout << "virus(exist)" << i << ": " <<
				"Infected: " << virus->currentNodeSet[i]->Infected.size()
				+ virus->currentNodeSet[i]->Recover.size() 
				+ virus->currentNodeSet[i]->Death.size() <<//��Ⱦ����
				" Stage: ";
			switch (virus->currentNodeSet[i]->stage)
			{
			case 0:cout << "INCUBATE"; break;
			case 1:cout << "BREAK   "; break;
			case 2:cout << "ISOLATE "; break;
			}//��Ⱦ״̬
			cout<< " Recover: " << virus->currentNodeSet[i]->Recover.size() <<//��������
			" Death: " << virus->currentNodeSet[i]->Death.size() << //��������
			" LagTime: ";//���ʱ��
			if ((virus->currentNodeSet[i]->timecount + virus->currentNodeSet[i]->begintime)/24
				< (virus->currentNodeSet[i]->VaccineTime + virus->currentNodeSet[i]->VaccineBeginTime / 24))
				cout << (virus->currentNodeSet[i]->VaccineTime + virus->currentNodeSet[i]->VaccineBeginTime / 24)
				- (virus->currentNodeSet[i]->timecount + virus->currentNodeSet[i]->begintime)/24;
			else cout << "-";

			cout << endl;
			virus->currentNodeSet[i]->Death.clear();
			virus->currentNodeSet[i]->Recover.clear();
			//��ÿ������������������������,�Ա�֤���������ֻ�ǵ�������
		}
	}
}
void SandBox::Recover(VirusNode* current, Human* hm) {//�����������������
	hm->mark[current->order] = 1;//���ýڵ���
	current->Recover.push_back(hm);//���ָ����������
	if (current->Infected.size() > 1) {//��������Ӳ�������и�Ⱦ�����༯ɾ��
		for (int j = 0; j < current->Infected.size(); j++) {
			if (hm == current->Infected[j]) {
				current->Infected.erase
				(current->Infected.begin() + j, current->Infected.begin() + j + 1);
			}
		}
	}
	else
		current->Infected.clear();
	VirusNode* temp = current;
	while (temp->PreVirus != NULL) {
		temp = temp->PreVirus;
		hm->mark[temp->order] = 1;
	}//��Ǹý�����游�������ᱻ�ٴθ�Ⱦ
	if (hm->InfectedVirus.size() > 1) {//�������Ӹ������Ⱦ�Ĳ�����ɾ��
		for (int j = 0; j < hm->InfectedVirus.size(); j++) {
			if (current == hm->InfectedVirus[j]) {
				hm->InfectedVirus.erase
				(hm->InfectedVirus.begin() + j, hm->InfectedVirus.begin() + j + 1);
			}
		}
	}
	else
		hm->InfectedVirus.clear();
}
void SandBox::Kill(VirusNode* current, Human* hm) {//������������
	current->Death.push_back(hm);//������������������༯��
	if (current->Infected.size() > 1) {//��������Ӳ����ĸ�Ⱦ���༯�Ͻ���ɾ��
		for (int j = 0; j < current->Infected.size(); j++) {
			if (hm == current->Infected[j]) {
				current->Infected.erase
				(current->Infected.begin() + j, current->Infected.begin() + j + 1);
			}
		}
	}
	else
		current->Infected.clear();
	if (HumanSet.size() > 1) {//���������ɳ������༯�н���ɾ��
		for (int i = 0; i < HumanSet.size(); i++) {
			if (hm == HumanSet[i]) {
				HumanSet.erase(HumanSet.begin() + i, HumanSet.begin() + i + 1);
			}	
		}
	}
	else
		HumanSet.clear();
}
void SandBox::SwicthState(int humanorder) {//״̬ת��:��������״̬ת��
	Human* person = HumanSet[humanorder];
	for (int i = 0; i < person->InfectedVirus.size(); i++) {
		VirusNode* curvirus = person->InfectedVirus[i];
		double rec, keep, die, state;
		rec = curvirus->DeathRate[0];
		keep = curvirus->DeathRate[1];
		die = curvirus->DeathRate[2];
		state = rand() / (double)(RAND_MAX);//����0~1�������
		if (state >= 0 && state <= rec) {//�����������ڻָ�����,���лָ�����
			Recover(curvirus, person);
		}
		else if (state <= rec + die) {//��������������������,������������
			Kill(curvirus, person);
		}
		//�����ڳ�������״̬
	}
}
VirusNode* SandBox::Variation(VirusNode* current) {//�����������������,Ȼ���ٱ���,���Զ�̬��ʡ�ռ�
	if (current->VariationTime <= current->VaccineTime && current->ifLeaf()) {
		//����ýڵ���Ҷ�ӽ��,������N���ӽ��,�����в��ҿɱ�����
		int k = 0;
		vector<VirusNode*>vari;//����������
		for (int i = 0; i < N; i++) {
			VirusNode* subnode = new VirusNode;//���ɱ�����(�������Ĺ���)
			current->subVirus[i] = subnode;
			subnode->PreVirus = current;
			if ((int)(current->timecount / 24)
				>= current->subVirus[i]->VariationTime) {
				//����:���ʱ����ڱ�������ʱ��,����ĳ���ڵ����
				k++;
				vari.push_back(current->subVirus[i]);//���ɱ���Ľ���ݴ�
			}
		}
		if (k > 0) {
			int random = rand() % k;//�ڿɱ�����ӽ�������ȡһ��
			current = vari[random];
			return current;
		}
	}
	else if (current->VariationTime <= current->VaccineTime && current->ifLeaf()) {
		//����ýڵ㲻��Ҷ�ӽ��,������ӽ���в��ҿɱ�����
		int k = 0;
		vector<VirusNode*>vari;//����������
		for (int i = 0; i < N; i++) {
			if ((int)(current->timecount / 24)
				>= current->subVirus[i]->VariationTime) {
				//����:���ʱ����ڱ�������ʱ��,����ĳ���ڵ����
				k++;
				vari.push_back(current->subVirus[i]);
			}
		}
		if (k > 0) {
			int random = rand() % k;//�ڿɱ�����ӽ�������ȡһ��
			current = vari[random];
			return current;
		}
	}
	return NULL;
}
void SandBox::Break(VirusNode* current) {//������Ⱦ����
	if ((int)(current->timecount / 24) >= current->Incubation && current->stage == 0) {
		current->stage = 1;//������״̬����Ϊ����״̬
		current->Infection *= 10;//��Ⱦ������Ϊԭ��10��
	}
}
void SandBox::Isolate(VirusNode* current) {
	if ((int)(current->timecount / 24 - current->Incubation) >= IsolationTime && current->stage == 1) {
		//�жϱ���:������ʱ����Ǳ���ڶԱ��жϲ����Ƿ���Ҫ����
		current->Infection = 0;//����֮���޷�������Ⱦ
		current->stage = 2;//������״̬����Ϊ����״̬
	}
}
void SandBox::UpdateMark() {//����ÿһ������ı��
	int tmp;
	for (int i = 0; i < HumanSet.size(); i++) {
		tmp = HumanSet[i]->mark.size();
		for (int j = 0; j < (virus->NodeSet.size() - tmp); j++)
			HumanSet[i]->mark.push_back(0);
		//������������,���ں󷽼����Ӧ��־,������Ϊδ��Ⱦ
	}
}
void SandBox::SwitchSituation() {
	//����Ȼ�����ǲ��е�,����Ӧ����������,��ÿ����㶼����״̬�任����
	for (int i = 0; i < virus->currentNodeSet.size(); i++) {
		if (virus->currentNodeSet[i]->ifVaccineSucc == false) {
			VirusNode* current = virus->currentNodeSet[i];//�Խڵ���б���
			if (current != NULL) {
				if (current->ifInfect) {
					current->timecount++;//�������Ⱦ��,��������ʱ��ת��
				}
				if (current->VariationTime <= current->timecount / 24
					&& current->timecount % 24 == 0) 
					//��Ⱦ�������ڱ�����������,����б������
				{	
					VirusNode* tmp = Variation(current);
					if (tmp != NULL) {
						virus->currentNodeSet.push_back(tmp);
						//�������첢���벡������
					}
				}
				virus->SaveEveryNode();//���²�������
				UpdateMark();//��ÿ�����ཫ������Ǹ���
				Infect(current);//���и�Ⱦ����
				for (int i = 0; i < HumanSet.size(); i++) {
					SwicthState(i);//��ÿһ���˽���״̬ת��
				}
				Vaccine(current);//��������
				Break(current);//��������
				Isolate(current);//��������
			}
		}
	}
}
VirusTree* SandBox::GetVirus() {
	return virus;
}

VirusTree::VirusTree() {
	root = new VirusNode;
	currentNodeSet.push_back(root);
}
VirusTree::~VirusTree() {
	DestroyTree(root);
}
void VirusTree::SaveEveryNode() {//��α����洢���Ľ��(�������ݽṹ)
	NodeSet.clear();
	VirusNode* current;
	queue<VirusNode*>tempqueue;//�ö����ݴ���,�������ǰ�����
	tempqueue.push(root);
	int order = 0;
	while (!tempqueue.empty())
	{
		current = tempqueue.front();
		tempqueue.pop();
		current->order = order;
		order++;
		NodeSet.push_back(current);
		for (int i = 0; i < N; i++) {
			if (current->subVirus[i] != NULL)
				tempqueue.push(current->subVirus[i]);
		}
	}
}
VirusNode* VirusTree::GetNearestAncestor(Union* Un, VirusNode* root, VirusNode* virus1, VirusNode* virus2) {
	//Tarjan�����㷨,���ڴ��ϵ��±���ʱ���Ѿ������ж�������Ƚ����,���α�������,���Ӷ�ΪO(n+q)
	if (root != NULL) {
		for (int i = 0; i < N; i++) {
			if (root->subVirus[i] != NULL) {
				GetNearestAncestor(Un, root->subVirus[i], virus1, virus2);//������ȱ����ڵ�
				Un->UnionJoin(root->order, root->subVirus[i]->order);//���������Ľ�����ͬһ�����鼯
				Un->UnionVis(root->subVirus[i]->order);//��ǽڵ��Ѿ�����
			}
		}
		if (virus1 == root && Un->UnionisVis(virus2->order)) {
			return NodeSet[Un->UnionFind(virus2->order)];//���������virus1,virus2��ǹ�,�򷵻�2�ĸ����
		}
		if (virus2 == root && Un->UnionisVis(virus1->order)) {
			return NodeSet[Un->UnionFind(virus1->order)];//����ͬ��
		}
	}
}
void VirusTree::DestroyTree(VirusNode* current) {
	if (current != NULL) {
		for (int i = 0; i < N; i++) {
			DestroyTree(current->subVirus[i]);//����������,���ڵ�ɾ��
		}
		delete current;
		current = NULL;
	}
}

Union::Union() {
	virus = NULL;
}
Union::~Union() {
	pre.clear();
	height.clear();
	vis.clear();
}
void Union::Initialize() {//��ʼ�����鼯
	for (int i = 0; i < virus->NodeSet.size(); i++) {
		pre.push_back(i);
		height.push_back(1);
		vis.push_back(0);
	}
}
void Union::SetTree(VirusTree* Tree) {
	virus = Tree;
}
int Union::UnionFind(int x) {//���ҽ��ĸ����
	if (pre[x] == x) return x;
	return pre[x] = UnionFind(pre[x]);
}
bool Union::UnionisConnected(int x, int y) {//�ж���������Ƿ���һ�����鼯
	return UnionFind(x) == UnionFind(y);
}
bool Union::UnionJoin(int x, int y) {//��x�ϲ���y�Ĳ��鼯
	x = UnionFind(x);
	y = UnionFind(y);
	if (x == y) return false;
	else {
		if (height[x] == height[y]) height[y]++;
		pre[x] = y;
	}
	return true;
}
void Union::UnionVis(int x) {
	vis[x] = 1;
}
bool Union::UnionisVis(int x) {
	if (vis[x] == 1) return true;
	else return false;
}
VirusTree* Union::GetVirus() {
	return virus;
}

/*����LCA�㷨�Ľ���*/
/*queue<node*>ancestor;
void FindAncestor(node* CurrentVirus, node* PreVirus) {//�����㷨�������ݺܴ�ʱЧ�ʺܵ�
	if (CurrentVirus != NULL) {
		if (CurrentVirus == PreVirus) {
			ancestor.push(PreVirus);
		}
		FindAncestor(CurrentVirus->PreVirus, PreVirus);
	}
}
node* VirusTree::GetNearestAncestor(node* virus1, node* virus2) {
	stack<node*>temp;
	temp.push(virus1);
	while (temp.top() != NULL) {
		node* pre = new node;
		pre = temp.top(); temp.pop();
		FindAncestor(virus2, pre);	
		temp.push(pre->PreVirus);
	}
	return ancestor.front();
}*/

/*���ɳ���ʵ��*/
/*int min(int a, int b) {
	if (a < b)return a;
	else return b;
}
void SwitchSandBox(SandBox A, SandBox B) {//ɳ��֮����Ա����
	for (int i = 0; i < min(A.GetHumanSet().size(), B.GetHumanSet().size()) / 10; i++) {
		int randomA = rand() % A.GetHumanSet().size();
		int randomB = rand() % B.GetHumanSet().size();
		B.GetHumanSet().push_back(A.GetHumanSet()[randomA]);
		A.GetHumanSet().push_back(B.GetHumanSet()[randomB]);
		A.GetHumanSet().erase
		(A.GetHumanSet().begin() + randomA, A.GetHumanSet().begin() + randomA + 1);
		B.GetHumanSet().erase
		(B.GetHumanSet().begin() + randomB, B.GetHumanSet().begin() + randomB + 1);
	}
	//����ʱ��ԭ��,����ɳ�о����㷨���������
	�ϲ���������ϲ�������:
	1.����Ŀ��,ɳ�����/����ṹ�Ĳ���/���鼯�Ĳ���ȫ���ǻ������Ľṹ����
	�����Ա����,����Ҫ�������������ɾ������(�����жϲ��������Ƿ�ȫ�������Ļ�����)
	�����ò�ͬ�����ĺϲ�ֻ�ǻ��ڲ��ֽ��,������ɵ����н�����޽���,��Ҫ�ж�;
	2.�����㷨���ʱ��ͬɳ�䲡�����������,
	���Ժϲ��������벡����Ĺ����ǻ��ڴ������ҵĻ�����,�Ա�֤�������Ĳ��غ�;
	3.���ڴ��������ͬ��ɳ��,����������ҲҪ���趨
	4.������Ĳ����������в��Ҹ����Ĳ���,������ԭ�в�������ʵ��
	5.���ÿ�����༰�������б���,���ж����н�����

}*/



int main() {
	srand((unsigned)time(NULL));//���ǲ���ɳ��ģ��
	cout << "Enter MAXDAY: " << endl;
	cin >> MAXDAY;
	cout << "Enter POPULATION: " << endl;
	cin >> POPULATION;
	VirusTree* Tree = new VirusTree;
	Tree->SaveEveryNode();
	Union* Un = new Union;
	SandBox* Sb = new SandBox;
	Sb->SetTree(Tree); 
	Sb->SetHuman(Tree);
	int date = 1;
	while (Sb->GetPopulation() > 0 || Sb->HourCnt % 24 != 0) {
		//ʱ��ת��������:����û�����ӻ��˿�������0
		Sb->Clkdiv();
		Sb->SwitchSituation();
		if (Sb->ifSucc() && Sb->HourCnt % 24 == 0)
			break;
	}
	if (Sb->ifSucc())//��������޷���Ⱦ����������,������ʤ��
		cout << endl << "Humankind wins!" << endl;
	else cout << endl << "No man suvives!" << endl;//�������ȫ������������������
	
	
	int a, b;//Ѱ�������������Ĵ��ڵĹ�������
	Un->SetTree(Sb->GetVirus());
	Un->Initialize();
	cout << "Enter 2 numbers of viruses: " << endl;
	cin >> a >> b;
	if (Un->GetVirus()->NodeSet.size() == 1 && 
		(a <= Un->GetVirus()->NodeSet.size() && b <= Un->GetVirus()->NodeSet.size())) 
	{//����ֻ��һ�����
		cout << "Tree has only one node!" << endl;
	}
	else if (a > Un->GetVirus()->NodeSet.size() || b > Un->GetVirus()->NodeSet.size()) {
		//��㲻����
		cout << "Order NOT exist!" << endl;
	}
	else {
		VirusNode* tmp = new VirusNode;
		stack<VirusNode*>stmp;
		for (int i = 0; i < Un->GetVirus()->currentNodeSet.size(); i++)
		{
			tmp = Un->GetVirus()->GetNearestAncestor
			(Un, Un->GetVirus()->NodeSet[0], Un->GetVirus()->NodeSet[a], Un->GetVirus()->NodeSet[b]);
			while (tmp != NULL) {
				//�����������ڴ��ڵĲ������У���������������Ѱ�Ҹ����
				tmp = tmp->PreVirus;
				if (tmp == Un->GetVirus()->currentNodeSet[i])
					stmp.push(tmp);
			}
		}//������Ľ��
		cout << "Nearest ancestor is: Node " << stmp.top()->order;
	}
}