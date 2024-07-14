#include"Date.h"

using std::cout; using std::endl;
using std::cin; using std::domain_error;

int main() {
	int year, month, day;
	cout << "请输入两个日期(年月日, 以空格分隔): " << endl;
	cin >> year >> month >> day;
	Date d1(year, month, day);//采用三个参数变量进行赋值
	try { d1.is_valid(); }
	catch(domain_error e) {
		cout << e.what() << endl;
		return 0;
	}
	Date d2(cin);//采用输入流赋值
	try { d2.is_valid(); }
	catch (domain_error e) {
		cout << e.what() << endl;
		return 0;
	}
	cout << "日期一: "; 
	d1.output();
	cout << "日期二: ";
	d2.output();
	cout << "两个日期相隔天数为: " << interval(d1, d2) << endl;
	Date d3;
	cout << "给日期一增加一天为: ";
	d3 = d1; d3.add_day(); d3.output();
	cout << "给日期一增加一月为: ";
	d3 = d1; d3.add_month(); d3.output();
	cout << "给日期一增加一年为: ";
	d3 = d1; d3.add_year(); d3.output();
	return 0;
}