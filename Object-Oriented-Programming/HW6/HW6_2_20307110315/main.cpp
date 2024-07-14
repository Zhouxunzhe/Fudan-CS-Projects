#include"Queue.h"

using std::cout; using std::endl;

int main() {
	int time = 0, serial = 1;
	cout << "          食堂某个窗口高峰期的排队状况模拟: " << endl;
	Queue<int> people;
	while (!people.isFull()) {
		if (time % 25 == 0) {
			if (!people.isEmpty()) {
				cout << "          " << time << "s: 第" << people.front() << "人出队." << endl;
				people.deQueue();
			}
		}
		if (time % 8 == 0) {
			people.enQueue(serial);
			cout << time << "s: 第" << serial << "人进队." << endl;
			serial++;
		}
		time++;
	}
	cout << "          该窗口排队人数已达到50人! 请分流! " << endl;
	cout << "      队首人员于" << (people.front() - 1) * 8 << "s入队." << endl;
	cout << "      队尾人员于" << (people.back() - 1) * 8 << "s入队." << endl;
	Queue<int> q1(people), q2; //people是原队列
	q1.output();
	q2 = q1;
	q2.output();
}