#include"Date.h"

using std::cout; using std::endl;
using std::cin; using std::domain_error;

int main() {
	int year, month, day;
	cout << "��������������(������, �Կո�ָ�): " << endl;
	cin >> year >> month >> day;
	Date d1(year, month, day);//�������������������и�ֵ
	try { d1.is_valid(); }
	catch(domain_error e) {
		cout << e.what() << endl;
		return 0;
	}
	Date d2(cin);//������������ֵ
	try { d2.is_valid(); }
	catch (domain_error e) {
		cout << e.what() << endl;
		return 0;
	}
	cout << "����һ: "; 
	d1.output();
	cout << "���ڶ�: ";
	d2.output();
	cout << "���������������Ϊ: " << interval(d1, d2) << endl;
	Date d3;
	cout << "������һ����һ��Ϊ: ";
	d3 = d1; d3.add_day(); d3.output();
	cout << "������һ����һ��Ϊ: ";
	d3 = d1; d3.add_month(); d3.output();
	cout << "������һ����һ��Ϊ: ";
	d3 = d1; d3.add_year(); d3.output();
	return 0;
}