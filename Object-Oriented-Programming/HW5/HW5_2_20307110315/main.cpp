#include"Queue.h"

using std::cout; using std::endl;

int main() {
	int time = 0, serial = 1;
	cout << "          食堂某个窗口高峰期的排队状况模拟: " << endl;
	Queue<int> people(50);
	while (!people.full()) {
		if (time % 25 == 0) {
			if (!people.empty()) {
				node<int>* p;
				p = people.front();
				cout << "          " << time << "s: 第" << p->data << "人出队." << endl;
				people.pop();
			}
		}
		if (time % 8 == 0) {
			people.push(serial);
			cout << time << "s: 第" << serial << "人进队." << endl;
			serial++;
		}
		time++;
	}
	cout << "          该窗口排队人数已达到50人! 请分流! " << endl;
	cout << "      队首人员于" << (people.front()->data - 1) * 8 << "s入队." << endl;
	cout << "      队尾人员于" << (people.back()->data - 1) * 8 << "s入队." << endl;
}