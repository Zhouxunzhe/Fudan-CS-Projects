#include<iostream>
#include<vector>
using std::cin; using std::vector;
using std::cout; using std::endl;

int mod(int& obj, int a, int b)
{
	if (a < 0 && a % b != 0) {
		obj = a / b + 1;
		return a - (a / b + 1) * b;//如果余数小于0，则返回一个非负余数
	}
	obj = a / b;
	return a % b;
}


int main() {
	int obj, base;
	vector<int>result;
	cout << "Enter a decimal integer([2^31,2^31-1]) and a base number([-16, -2]): " << endl;
	cin >> obj >> base;
	int mem = obj;
	while (obj != 0){
		result.push_back(mod(obj, obj, base));
	}
	int sz = result.size();
	cout << mem << "=";
	for (int i = sz - 1; i >= 0; i--) {
		if (result[i] < 10)cout << result[i];
		else {
			switch (result[i]) {
			case 10:cout << 'A'; break;
			case 11:cout << 'B'; break;
			case 12:cout << 'C'; break;
			case 13:cout << 'D'; break;
			case 14:cout << 'E'; break;
			case 15:cout << 'F'; break;
			}
		}
	}
	cout << "(base" << base << ")";
}