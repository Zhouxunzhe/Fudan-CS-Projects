#ifndef GUARD_Date_h
#define GUARD_Date_h

#include<iostream>
#include<stdexcept>

class Date {
public:
	Date() :year(0), month(0), day(0) {}
	explicit Date(int y, int m, int d) { year = y; month = m; day = d; }
	explicit Date(std::istream& in) { in >> year >> month >> day; }
	int get_year() const { return year; }
	int get_month() const { return month; }
	int get_day() const { return day; }
	void output() { std::cout << year << "Äê" << month << "ÔÂ" << day << "ÈÕ" << std::endl; }
	void add_year();
	void add_month();
	void add_day();
	bool is_leap();
	void is_valid();
private:
	int year, month, day;
};
int interval(Date, Date);

#endif // !GUARD_Date_h
