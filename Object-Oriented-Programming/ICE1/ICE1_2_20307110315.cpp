#include<iostream>
using std::cout; using std::cin;

int main() {
	int a, n, m, x;
	int in1, in2, tmp, b;
	int cnt, cntx;
	cout << "输入始发站上车人数,车站数,终点站下车人数和所求站点编号: ";
	cin >> a >> n >> m >> x;
	in1 = a; in2 = 0; cnt = a;
	for (int i = 2; i < n-1; i++) {
		cnt += in2 + in1;
		cnt -= in2;
		tmp = in1;
		in1 = in2;
		in2 = tmp + in2;
	}
	int u = 1, v = 2;
	cntx = 0;
	if (n < 4) {
		cntx = 0;
	}
	else if (n == 4) cntx = u;
	else if (n == 5) cntx = v;
	else {
		for (int i = 5; i < n - 1; i++) {
			cntx = u + v + 1;
			u = v;
			v = cntx;
		}
	}
	b = (m - cnt) / cntx;
	in1 = a; in2 = b; cnt = a;
	for (int i = 2; i < x; i++) {
		cnt += in2 + in1;
		cnt -= in2;
		tmp = in1;
		in1 = in2;
		in2 = tmp + in2;
	}
	if (x == n) cnt = 0;
	cout << "第" << x << "站开出时车上的人数是: " << cnt;
	return 0;
}