#include"Queue.h"

using std::cout; using std::endl;

int main() {
	int time = 0, serial = 1;
	cout << "          ʳ��ĳ�����ڸ߷��ڵ��Ŷ�״��ģ��: " << endl;
	Queue<int> people(50);
	while (!people.full()) {
		if (time % 25 == 0) {
			if (!people.empty()) {
				node<int>* p;
				p = people.front();
				cout << "          " << time << "s: ��" << p->data << "�˳���." << endl;
				people.pop();
			}
		}
		if (time % 8 == 0) {
			people.push(serial);
			cout << time << "s: ��" << serial << "�˽���." << endl;
			serial++;
		}
		time++;
	}
	cout << "          �ô����Ŷ������Ѵﵽ50��! �����! " << endl;
	cout << "      ������Ա��" << (people.front()->data - 1) * 8 << "s���." << endl;
	cout << "      ��β��Ա��" << (people.back()->data - 1) * 8 << "s���." << endl;
}