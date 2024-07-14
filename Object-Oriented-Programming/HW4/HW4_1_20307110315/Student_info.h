#ifndef GUARD_Student_info
#define GUARD_Student_info

#include<iostream>
#include<string>
#include<vector>

class Student_info {
public:
	Student_info() :midterm(0), final(0) { }
	explicit Student_info(std::istream& is) { read(is); }
	std::string name() const { return n; }
	bool valid() const { return !homework.empty(); }
	std::istream& read(std::istream&);
	double grade() const;
private:
	std::string n;
	double midterm, final;
	std::vector<double> homework;
};
bool compare(const Student_info&, const Student_info&);


#endif // !GUARD_Student_info