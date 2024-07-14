#include"Queue.h"

using std::cout; using std::endl;

int main() {
	int time = 0, serial = 1;
	cout << "          ʳ��ĳ�����ڸ߷��ڵ��Ŷ�״��ģ��: " << endl;
	Queue<int> people;
	while (!people.isFull()) {
		if (time % 25 == 0) {
			if (!people.isEmpty()) {
				cout << "          " << time << "s: ��" << people.front() << "�˳���." << endl;
				people.deQueue();
			}
		}
		if (time % 8 == 0) {
			people.enQueue(serial);
			cout << time << "s: ��" << serial << "�˽���." << endl;
			serial++;
		}
		time++;
	}
	cout << "          �ô����Ŷ������Ѵﵽ50��! �����! " << endl;
	cout << "      ������Ա��" << (people.front() - 1) * 8 << "s���." << endl;
	cout << "      ��β��Ա��" << (people.back() - 1) * 8 << "s���." << endl;
	Queue<int> q1(people), q2; //people��ԭ����
	q1.output();
	q2 = q1;
	q2.output();
}