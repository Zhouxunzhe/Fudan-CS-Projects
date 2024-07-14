#include<iostream>
#include<string>
using std::string; using std::cout;
using std::cin; using std::endl;

int main() {
	string name; int blank;
	cout << "please enter your first name: ";
	cin >> name;
	cout << "please enter the number of blanks surrounding the greeting: ";
	cin >> blank;
	const string greeting("hello " + name + " !");
	cout << endl;
	for (int i = 0; i < 3 + 2 * blank; i++) {
		if (i == 0 || i == 3 + 2 * blank - 1) {
			for (int j = 0; j < 2 * (1 + blank) + greeting.length(); j++) {
				cout << "*";
			}
			cout << endl;
		}
		else if (i == 1 + blank) {
			cout << "*";
			for (int j = 0; j < blank; j++) {
				cout << " ";
			}
			cout << greeting;
			for (int j = 0; j < blank; j++) {
				cout << " ";
			}
			cout << "*" << endl;
		}
		else {
			cout << "*";
			for (int j = 0; j < 2 * blank + greeting.length(); j++) {
				cout << " ";
			}
			cout << "*" << endl;
		}
	}
	return 0;
}