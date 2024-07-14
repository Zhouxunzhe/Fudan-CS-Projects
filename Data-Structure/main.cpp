#include"Human.h"
#include"Node.h"
#include"Tree.h"
#include"Union.h"
#include"SandBox.h"
using namespace std;
#define N 10//结点度数
int MAXDAY;//潜伏期,变异时间,疫苗研制时间所需最大整数
int POPULATION;//沙箱人口总数

Human::~Human() {
	InfectedVirus.clear();
}
Human::Human() {
	virus = NULL;
}
void Human::Initialize(VirusTree* Tree) {//内置树初始化
	virus = Tree;
}

VirusNode::VirusNode() {//每个结点进行初始化
	Incubation = rand() % MAXDAY;//潜伏期的设定范围为0~MAXDAY的随机数
	Infection = rand() / (double)(RAND_MAX) * POPULATION;
	//传染率的设定范围为0~POPULATION的double类型的随机数
	double a = rand() % 100, b = rand() % 100, c = rand() % 100;
	double total = a + b + c;
	DeathRate[0] = a / total; DeathRate[1] = b / total; DeathRate[2] = c / total;
	//致死率的设定随机生成,三者概率总和加起来为1
	//DeathRate[0]表示康复率
	//DeathRate[2]表示持续治疗概率
	//DeathRate[3]表示死亡率
	VariationTime = rand() % MAXDAY;//变异所需的时间范围为0~MAXDAY的随机数
	order = 0;
	Infected.clear(); Recover.clear(); Death.clear();
	stage = 0;
	timecount = 0;
	begintime = 0;
	VaccineBeginTime = 0;
	VaccineTime = rand() % MAXDAY;//疫苗研制所需时间范围为0~MAXDAY的随机数
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
bool VirusNode::ifLeaf() {//如果所有子结点为NULL则该节点为叶子结点
	for (int i = 0; i < N; i++) {
		if (subVirus[i] != NULL) {
			return false;
		}
	}
	return true;
}

double GaussRand(int Infection) {//高斯函数,得到正态分布的随机数
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
	return (x * 0.5 + Infection);//期望为感染率，标准差为100
}
SandBox::SandBox() {
	virus = new VirusTree;
	HumanSet.clear();
	Population = POPULATION;//定义人数
	IsolationTime = rand() % MAXDAY;//爆发到隔离所需时间随机生成
	HourCnt = 0; DayCnt = 0;//沙箱内置时钟
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
bool SandBox::ifSucc(){//通过病毒能否继续传染判断人类是否胜利
	for (int i = 0; i < virus->currentNodeSet.size(); i++) {
		if (virus->currentNodeSet[i]->Infection != 0)
			return false;
	}
	if (HumanSet.size() > 0)
		return true;
	else
		return false;
}
void SandBox::Vaccine(VirusNode* current) {//进行疫苗的研制并设置病毒疫苗研制状态
	if ((current->timecount + current->begintime) /24
		>= (current->VaccineTime + current->VaccineBeginTime / 24)) {
		current->ifVaccineSucc = true;//疫苗研制完成
		current->Infection = 0;//将病毒传染率设置为0,即不能继续传染
		if (current->VariationTime != 0)
			current->VariationTime = rand() % current->VariationTime + current->VariationTime + 1;
		//疫苗研制成功后变异时间将随机增加为VariationTime~2*VariationTime区间
		for (int i = 0; i < current->Infected.size(); i++) {//每一个病人康复
			Recover(current, current->Infected[i]);
		}
	}
}
void SandBox::Infect(VirusNode* current) {//进行病毒的传播过程
	if (current->ifInfect == false) {
		current->ifInfect = true;//设置病毒传染状态
		current->begintime = HourCnt;//初始化病毒内置时钟
		current->VaccineBeginTime = rand() % current->begintime;
		//研制疫苗假定在刚传染或之前就开始进行
	}
	if (current->ifVaccineSucc == false) {
		int intInf = (int)GaussRand(current->Infection);
		//设置感染人数为围绕期望值(传染率)正太分布的数值
		if(intInf<=HumanSet.size()){//对人类进行随机传染(可以重复传染)
			for (int i = 0; i < intInf; i++) {
				int random = rand() % HumanSet.size();//设置随机数,对不超过传染率统计值的人数进行传染
				if (HumanSet[random]->mark[current->order] != 1) {//判断是否已经感染过该病毒
					HumanSet[random]->InfectedVirus.push_back(current);
					HumanSet[random]->mark[current->order] = 1;
					current->Infected.push_back(HumanSet[random]);
				}
			}
		}
		else//如果传染率高于存活的人数,则对每一个存活的人进行传染
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
void SandBox::SetHuman(VirusTree* Tree) {//设置总的人类集合
	for (int i = 0; i < Population; i++) {
		Human* hm = new Human;
		hm->Initialize(Tree);//对每一个人类进行内置树的初始化
		HumanSet.push_back(hm);
	}
}
void SandBox::Clkdiv() {//时针跳动,如果天数改变则输出每天的沙箱模拟情况
	HourCnt++;//时针开始跳动
	if (HourCnt % 24 == 0) {
		DayCnt++;//如果时针转过24小时,则天数进一位
		cout << "Day" << DayCnt << ": " << endl;
		for (int i = 0; i < virus->currentNodeSet.size(); i++) {
			cout << "virus(exist)" << i << ": " <<
				"Infected: " << virus->currentNodeSet[i]->Infected.size()
				+ virus->currentNodeSet[i]->Recover.size() 
				+ virus->currentNodeSet[i]->Death.size() <<//感染人数
				" Stage: ";
			switch (virus->currentNodeSet[i]->stage)
			{
			case 0:cout << "INCUBATE"; break;
			case 1:cout << "BREAK   "; break;
			case 2:cout << "ISOLATE "; break;
			}//感染状态
			cout<< " Recover: " << virus->currentNodeSet[i]->Recover.size() <<//治愈人数
			" Death: " << virus->currentNodeSet[i]->Death.size() << //死亡人数
			" LagTime: ";//落后时间
			if ((virus->currentNodeSet[i]->timecount + virus->currentNodeSet[i]->begintime)/24
				< (virus->currentNodeSet[i]->VaccineTime + virus->currentNodeSet[i]->VaccineBeginTime / 24))
				cout << (virus->currentNodeSet[i]->VaccineTime + virus->currentNodeSet[i]->VaccineBeginTime / 24)
				- (virus->currentNodeSet[i]->timecount + virus->currentNodeSet[i]->begintime)/24;
			else cout << "-";

			cout << endl;
			virus->currentNodeSet[i]->Death.clear();
			virus->currentNodeSet[i]->Recover.clear();
			//将每天的死亡人数与治愈人数清空,以保证当天输出的只是当天的情况
		}
	}
}
void SandBox::Recover(VirusNode* current, Human* hm) {//对人类进行治愈操作
	hm->mark[current->order] = 1;//将该节点标记
	current->Recover.push_back(hm);//将恢复的人类存入
	if (current->Infected.size() > 1) {//将该人类从病毒结点中感染的人类集删除
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
	}//标记该结点与祖父结点均不会被再次感染
	if (hm->InfectedVirus.size() > 1) {//将病毒从该人类感染的病毒集删除
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
void SandBox::Kill(VirusNode* current, Human* hm) {//进行死亡操作
	current->Death.push_back(hm);//将该人类存入死亡人类集合
	if (current->Infected.size() > 1) {//将该人类从病毒的感染人类集合进行删除
		for (int j = 0; j < current->Infected.size(); j++) {
			if (hm == current->Infected[j]) {
				current->Infected.erase
				(current->Infected.begin() + j, current->Infected.begin() + j + 1);
			}
		}
	}
	else
		current->Infected.clear();
	if (HumanSet.size() > 1) {//将该人类从沙箱的人类集中进行删除
		for (int i = 0; i < HumanSet.size(); i++) {
			if (hm == HumanSet[i]) {
				HumanSet.erase(HumanSet.begin() + i, HumanSet.begin() + i + 1);
			}	
		}
	}
	else
		HumanSet.clear();
}
void SandBox::SwicthState(int humanorder) {//状态转移:个体生存状态转移
	Human* person = HumanSet[humanorder];
	for (int i = 0; i < person->InfectedVirus.size(); i++) {
		VirusNode* curvirus = person->InfectedVirus[i];
		double rec, keep, die, state;
		rec = curvirus->DeathRate[0];
		keep = curvirus->DeathRate[1];
		die = curvirus->DeathRate[2];
		state = rand() / (double)(RAND_MAX);//设置0~1的随机数
		if (state >= 0 && state <= rec) {//如果随机数落在恢复区间,进行恢复操作
			Recover(curvirus, person);
		}
		else if (state <= rec + die) {//如果随机数落在死亡区间,进行死亡操作
			Kill(curvirus, person);
		}
		//否则处于持续治疗状态
	}
}
VirusNode* SandBox::Variation(VirusNode* current) {//变异过程是先生成树,然后再变异,可以动态节省空间
	if (current->VariationTime <= current->VaccineTime && current->ifLeaf()) {
		//如果该节点是叶子结点,则生成N个子结点,并从中查找可变异结点
		int k = 0;
		vector<VirusNode*>vari;//定义变异概率
		for (int i = 0; i < N; i++) {
			VirusNode* subnode = new VirusNode;//生成变异结点(创建树的过程)
			current->subVirus[i] = subnode;
			subnode->PreVirus = current;
			if ((int)(current->timecount / 24)
				>= current->subVirus[i]->VariationTime) {
				//变异:如果时针大于变异所需时间,则向某个节点变异
				k++;
				vari.push_back(current->subVirus[i]);//将可变异的结点暂存
			}
		}
		if (k > 0) {
			int random = rand() % k;//在可变异的子结点中随机取一个
			current = vari[random];
			return current;
		}
	}
	else if (current->VariationTime <= current->VaccineTime && current->ifLeaf()) {
		//如果该节点不是叶子结点,则从其子结点中查找可变异结点
		int k = 0;
		vector<VirusNode*>vari;//定义变异概率
		for (int i = 0; i < N; i++) {
			if ((int)(current->timecount / 24)
				>= current->subVirus[i]->VariationTime) {
				//变异:如果时针大于变异所需时间,则向某个节点变异
				k++;
				vari.push_back(current->subVirus[i]);
			}
		}
		if (k > 0) {
			int random = rand() % k;//在可变异的子结点中随机取一个
			current = vari[random];
			return current;
		}
	}
	return NULL;
}
void SandBox::Break(VirusNode* current) {//病毒传染爆发
	if ((int)(current->timecount / 24) >= current->Incubation && current->stage == 0) {
		current->stage = 1;//将病毒状态设置为爆发状态
		current->Infection *= 10;//传染率设置为原来10倍
	}
}
void SandBox::Isolate(VirusNode* current) {
	if ((int)(current->timecount / 24 - current->Incubation) >= IsolationTime && current->stage == 1) {
		//判断爆发:将爆发时间与潜伏期对比判断病毒是否需要隔离
		current->Infection = 0;//隔离之后将无法继续传染
		current->stage = 2;//将病毒状态设置为隔离状态
	}
}
void SandBox::UpdateMark() {//更新每一个人类的标记
	int tmp;
	for (int i = 0; i < HumanSet.size(); i++) {
		tmp = HumanSet[i]->mark.size();
		for (int j = 0; j < (virus->NodeSet.size() - tmp); j++)
			HumanSet[i]->mark.push_back(0);
		//如果有新增结点,则在后方加入对应标志,并设置为未感染
	}
}
void SandBox::SwitchSituation() {
	//变异等活动由于是并行的,所以应遍历整颗树,对每个结点都进行状态变换操作
	for (int i = 0; i < virus->currentNodeSet.size(); i++) {
		if (virus->currentNodeSet[i]->ifVaccineSucc == false) {
			VirusNode* current = virus->currentNodeSet[i];//对节点进行遍历
			if (current != NULL) {
				if (current->ifInfect) {
					current->timecount++;//如果结点感染过,则其内置时针转动
				}
				if (current->VariationTime <= current->timecount / 24
					&& current->timecount % 24 == 0) 
					//感染天数大于变异所需天数,则进行变异操作
				{	
					VirusNode* tmp = Variation(current);
					if (tmp != NULL) {
						virus->currentNodeSet.push_back(tmp);
						//病毒变异并存入病毒库中
					}
				}
				virus->SaveEveryNode();//更新病毒结点库
				UpdateMark();//对每个人类将病毒标记更新
				Infect(current);//进行感染操作
				for (int i = 0; i < HumanSet.size(); i++) {
					SwicthState(i);//对每一个人进行状态转移
				}
				Vaccine(current);//生产疫苗
				Break(current);//病毒爆发
				Isolate(current);//隔离治疗
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
void VirusTree::SaveEveryNode() {//层次遍历存储树的结点(树的数据结构)
	NodeSet.clear();
	VirusNode* current;
	queue<VirusNode*>tempqueue;//用队列暂存结点,后端输入前端输出
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
	//Tarjan离线算法,由于从上到下遍历时就已经可以判断最近祖先结点了,单次遍历即可,复杂度为O(n+q)
	if (root != NULL) {
		for (int i = 0; i < N; i++) {
			if (root->subVirus[i] != NULL) {
				GetNearestAncestor(Un, root->subVirus[i], virus1, virus2);//深度优先遍历节点
				Un->UnionJoin(root->order, root->subVirus[i]->order);//将遍历到的结点存入同一个并查集
				Un->UnionVis(root->subVirus[i]->order);//标记节点已经遍历
			}
		}
		if (virus1 == root && Un->UnionisVis(virus2->order)) {
			return NodeSet[Un->UnionFind(virus2->order)];//如果遍历到virus1,virus2标记过,则返回2的根结点
		}
		if (virus2 == root && Un->UnionisVis(virus1->order)) {
			return NodeSet[Un->UnionFind(virus1->order)];//如上同理
		}
	}
}
void VirusTree::DestroyTree(VirusNode* current) {
	if (current != NULL) {
		for (int i = 0; i < N; i++) {
			DestroyTree(current->subVirus[i]);//遍历整棵树,将节点删除
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
void Union::Initialize() {//初始化并查集
	for (int i = 0; i < virus->NodeSet.size(); i++) {
		pre.push_back(i);
		height.push_back(1);
		vis.push_back(0);
	}
}
void Union::SetTree(VirusTree* Tree) {
	virus = Tree;
}
int Union::UnionFind(int x) {//查找结点的根结点
	if (pre[x] == x) return x;
	return pre[x] = UnionFind(pre[x]);
}
bool Union::UnionisConnected(int x, int y) {//判断两个结点是否在一个并查集
	return UnionFind(x) == UnionFind(y);
}
bool Union::UnionJoin(int x, int y) {//将x合并到y的并查集
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

/*关于LCA算法的解释*/
/*queue<node*>ancestor;
void FindAncestor(node* CurrentVirus, node* PreVirus) {//在线算法，当数据很大时效率很低
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

/*多个沙箱的实现*/
/*int min(int a, int b) {
	if (a < b)return a;
	else return b;
}
void SwitchSandBox(SandBox A, SandBox B) {//沙箱之间人员流动
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
	//由于时间原因,交换沙盒具体算法用文字替代
	合并病毒树与合并病毒库:
	1.本项目中,沙箱操作/人类结构的操作/并查集的操作全部是基于树的结构进行
	如果人员流动,则需要引入树的添加与删除操作(基于判断病毒宿主是否全部交换的基础上)
	且两棵不同的树的合并只是基于部分结点,因此生成的树有交叉或无交叉,需要判断;
	2.由于算法设计时不同沙箱病毒树的随机性,
	所以合并病毒树与病毒库的工作是基于大量查找的基础上,以保证病毒结点的不重合;
	3.对于存活人数不同的沙箱,交换的人数也要有设定
	4.交换后的病毒树由于有查找根结点的操作,可以在原有病毒树上实现
	5.需对每个人类及病毒进行遍历,以判断所有结点情况

}*/



int main() {
	srand((unsigned)time(NULL));//蓝星病毒沙箱模拟
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
		//时针转动的条件:天数没有增加或人口数大于0
		Sb->Clkdiv();
		Sb->SwitchSituation();
		if (Sb->ifSucc() && Sb->HourCnt % 24 == 0)
			break;
	}
	if (Sb->ifSucc())//如果病毒无法传染且有人类存活,则人类胜利
		cout << endl << "Humankind wins!" << endl;
	else cout << endl << "No man suvives!" << endl;//如果人类全部死亡，则无人生还
	
	
	int a, b;//寻找两个结点最近的存在的公共祖先
	Un->SetTree(Sb->GetVirus());
	Un->Initialize();
	cout << "Enter 2 numbers of viruses: " << endl;
	cin >> a >> b;
	if (Un->GetVirus()->NodeSet.size() == 1 && 
		(a <= Un->GetVirus()->NodeSet.size() && b <= Un->GetVirus()->NodeSet.size())) 
	{//树中只有一个结点
		cout << "Tree has only one node!" << endl;
	}
	else if (a > Un->GetVirus()->NodeSet.size() || b > Un->GetVirus()->NodeSet.size()) {
		//结点不存在
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
				//如果最近祖先在存在的病毒集中，则输出，否则继续寻找根结点
				tmp = tmp->PreVirus;
				if (tmp == Un->GetVirus()->currentNodeSet[i])
					stmp.push(tmp);
			}
		}//输出最后的结点
		cout << "Nearest ancestor is: Node " << stmp.top()->order;
	}
}