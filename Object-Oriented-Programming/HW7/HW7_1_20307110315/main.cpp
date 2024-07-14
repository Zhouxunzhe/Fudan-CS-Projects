#include"Str.h"
using std::cout;
using std::cin; using std::endl;

int main() {
	Str name; int blank;
	cout << "please enter your first name: ";
	cin >> name;
	cout << "please enter the number of blanks surrounding the greeting: ";
	cin >> blank;
	const Str greeting("Hello, " + name + "!");
	cout << endl;
	for (int i = 0; i < 3 + 2 * blank; i++) {
		if (i == 0 || i == 3 + 2 * blank - 1) {
			for (Str::size_type j = 0; j < (Str::size_type)(2 * (1 + blank)) + greeting.size(); j++) {
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
			for (Str::size_type j = 0; j < (Str::size_type)(2 * blank) + greeting.size(); j++) {
				cout << " ";
			}
			cout << "*" << endl;
		}
	}
	cout << "逐个字符输出greeting(字符之间以空格分隔): " << endl;
	for (int i = 0; i < greeting.size(); i++) {
		cout << greeting[i] << " ";
	}
	return 0;
}