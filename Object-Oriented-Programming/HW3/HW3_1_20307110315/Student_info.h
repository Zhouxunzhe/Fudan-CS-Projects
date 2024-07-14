#ifndef GUARD_Student_info_h
#define GUARD_Student_info_h

#include<iostream>
#include<string>
#include<list>
#include<vector>

struct Student_info {
	std::string name;
	double midterm, final;
	std::vector<double>homework;
	double grade;
};
bool compare(const Student_info&, const Student_info&);
std::istream& read_hw(std::istream&, std::vector<double>&);
std::istream& read(std::istream&, Student_info&);
bool fagrade(const Student_info&);
std::list<Student_info>extract_fails(std::list<Student_info>&);
#endif // !GUARD_Student_info_h