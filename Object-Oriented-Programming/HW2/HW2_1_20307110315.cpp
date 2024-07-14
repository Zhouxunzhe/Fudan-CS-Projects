#include<iostream>
#include<string>
#include<string.h>
#include<vector>

using std::cin; using std::istream;
using std::string; using std::vector;
using std::cout; using std::domain_error;
using std::endl;

istream& read_string(istream& in, vector<string>& vs) {
	string s;
	vs.clear();
	while (in >> s) {
		vs.push_back(s);
	}
	return in;
}

bool strcmp(string str1, string str2) {
	if (str1.size() != str2.size()) return false;
	else {
		for (int i = 0; i < str1.size(); i++) {
			if (str1[i] != str2[i] &&
				str1[i] - 'a' + 'A' != str2[i] &&
				str1[i] != str2[i] - 'a' + 'A')
				return false;
		}
	}
	return true;
}

void get_num(const vector<string>& vs, vector<int>& count) {
	count.clear();
	for (int i = 0; i < vs.size(); i++) {
		count.push_back(0);
		for (int j = 0; j < vs.size(); j++) {
			if (strcmp(vs[j], vs[i])) {
				count[i]++;
			}
		}
	}
}

void output(const vector<string>& vs,vector<int> count) {
	if (vs.size() == 0) {
		throw domain_error("NO WORD ENTERED!");
	}
	else {
		cout << "number of the words: " << vs.size() << endl;
		int maxlength = 0;
		for (int i = 0; i < vs.size(); i++) {
			if (vs[i].length() > maxlength)
				maxlength = vs[i].length();
		}
		for (int i = 0; i < vs.size(); i++) {
			if (count[i] > 1) {//比较是否已经输出过
				for (int j = 0; j < i; j++) {
					if(strcmp(vs[j], vs[i])) {
						count[i] = 0;
					}
				}
			}
			if (count[i] != 0) {
				cout << vs[i];
				for (int j = 0; j < maxlength - vs[i].length(); j++) {
					cout << " ";
				}
				cout << " " << count[i] << endl;
			}
		}
	}
}

int main() {
	cout << "Enter some English words: " << endl;
	vector<string>words; vector<int>count;
	read_string(cin, words);
	get_num(words, count);
	try { output(words, count); }
	catch (domain_error) {
		cout << "No words entered."
			"Please try again!" ;
	}
}