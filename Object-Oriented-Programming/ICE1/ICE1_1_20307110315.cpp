#include<iostream>
#include<string>
using std::string; using std::cin;
using std::cout; using std::endl;

int main() {
	string s1, s2, s3;
	cout << "Enter 3 English words: ";
	cin >> s1 >> s2 >> s3;
	string tmp = s1;
	if (s2 < s1) {
		tmp = s2;
		s2 = s1;
		s1 = tmp;
	}
	if (s3 < s1) {
		tmp = s3;
		s3 = s1;
		s1 = tmp;
	}
	if (s3 < s2) {
		tmp = s3;
		s3 = s2;
		s2 = tmp;
	}
	cout << s1 << " " << s2 << " " << s3 << " " << endl;
	return 0;
}