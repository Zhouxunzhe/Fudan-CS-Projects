#include"head.h"
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<list>
#define N 13
#define MAX 20
using std::vector; using std::list;
using std::cin; using std::cout;
using std::endl; using std::string;

int main() {
	vector<int>v1, v2, vec_tmp;
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++) {
		v1.push_back(rand() % MAX);
		v2.push_back(rand() % MAX);
	}
	cout << "Random integer vector 1: " << endl;
	for (int i = 0; i < N; i++) {
		cout << v1[i] << "  ";
	}
	cout << endl;
	cout << "Random integer vector 2: " << endl;
	for (int i = 0; i < N; i++) {
		cout << v2[i] << "  ";
	}
	cout << endl;
	cout << "Vector in common:" << endl;
	vec_tmp = search(v1, v2.begin(), v2.end());
	output(vec_tmp);

	list<string>l1, l2, lis_tmp;
	string word;
	cout << "Enter some words for string list 1: " << endl;
	while (cin >> word)
		l1.push_back(word);
	cin.clear();
	cout << "Enter some words for string list 2: " << endl;
	while (cin >> word) 
		l2.push_back(word);
	cin.clear();
	cout << "List in common:" << endl;
	lis_tmp = search(l1, l2.begin(), l2.end());
	output(lis_tmp);
	
	return 0;
}