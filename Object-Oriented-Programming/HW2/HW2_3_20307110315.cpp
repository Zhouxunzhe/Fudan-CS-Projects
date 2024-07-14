#include<iostream>
#include<vector>
using std::vector; using std::cout;
using std::cin; using std::endl;

int max(int a, int b) {
	return a > b ? a : b;
}

vector<int> add(vector<int> res1, vector<int> res2) {
	int sig = 0, maxlen;
	maxlen = max(res1.size(), res2.size());
	int sz1 = res1.size(), sz2 = res2.size();
	for (int i = 0; i <= maxlen - sz1; i++) {
		res1.push_back(0);
	}
	for (int i = 0; i <= maxlen - sz2; i++) {
		res2.push_back(0);
	}
	for (int i = 0; i < maxlen + 1; i++) {
		int tmp = res1[i] + res2[i] + sig;
		res1[i] = tmp % 10;
		sig = tmp / 10;
	}
	return res1;
}

void output(vector<int> result) {
	int ifcout = 0; int cnt;
	for (cnt = result.size() - 1; cnt >= 0; cnt--) {
		if (result[cnt] != 0) {
			break;
		}
	}
	for (int i = cnt; i >= 0; i--) {
		cout << result[i];
	}
}

vector<int> multi(int n) {
	vector<int> res, tmp;
	res.push_back(1);
	for (int i = 1; i <= n; i++) {
		tmp = res;
		for (int j = 0; j < i - 1; j++) {
			res = add(tmp, res);
		}
	}
	return res;
}

int main() {
	int n;
	vector<int>res1, res2, result,mult;
	cout << "Enter n(1<=n<=50): ";
	cin >> n;
	for (int i = 1; i <= n; i++) {
		mult = multi(i);
		result = add(mult, result);
	}
	for (int i = 1; i < n; i++) {
		cout << i << "!+";
	}
	cout << n << "!=";
	output(result);
	cout << endl;
}
 