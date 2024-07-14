#include"Str.h"

using std::cin; using std::istream;
using std::cout; using std::domain_error;
using std::endl;

istream& read_string(istream& in, Vec<Str>& vs) {
	Str s;
	vs.clear();
	while (in >> s) {
		vs.push_back(s);
	}
	return in;
}

bool strcmp(Str str1, Str str2) {
	if (str1.size() != str2.size()) return false;
	else {
		for (int i = 0; i < str1.size(); i++) {
			if (str1[i] != str2[i]/* &&
				str1[i] - 'a' + 'A' != str2[i] &&
				str1[i] != str2[i] - 'a' + 'A'*/)
				//考虑大小写(即大写与小写有大小区分)
				return false;
		}
	}
	return true;
}

void get_num(const Vec<Str>& vs, Vec<int>& count) {
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

void output(const Vec<Str>& vs,Vec<int> count) {
	if (vs.size() == 0) {
		throw domain_error("NO TEXT ENTERED!");
	}
	else {
		int maxlength = 0;
		for (int i = 0; i < vs.size(); i++) {
			if (vs[i].size() > maxlength)
				maxlength = vs[i].size();
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
				for (int j = 0; j < maxlength - vs[i].size(); j++) {
					cout << " ";
				}
				cout << " " << count[i] << endl;
			}
		}
	}
}
bool compare(const Str& x, const Str& y) {
	Str::size_type sz1, sz2, size;
	sz1 = x.size(); 
	sz2 = y.size();
	//size = min{sz1, sz2}
	if (sz1 < sz2) size = sz1;
	else size = sz2;
	Str::size_type roll = 0;
	while (roll < size) {
		if (x[roll] < y[roll])return true;
		else if (x[roll] > y[roll])return false;
		else roll++;
	}
	return sz1 < sz2;
}

int main() {
	cout << "Enter some text: ";
	Vec<Str>words; Vec<int>count;
	read_string(cin, words);
	std::sort(words.begin(), words.end(), compare);
	get_num(words, count);
	try { output(words, count); }
	catch (domain_error) {
		cout << "No words entered."
			"Please try again!" ;
	}
}