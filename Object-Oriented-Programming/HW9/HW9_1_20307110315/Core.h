#pragma once
#ifndef Core_h
#define Core_h
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>

class Core {
public:
	Core() :midterm(0), final(0) { }
	Core(std::istream& is) { read(is); }
	virtual ~Core() { }
	std::string name() const;
	virtual double grade() const;
	virtual std::istream& read(std::istream&);
	friend class Student_info;
protected:
	std::istream& read_common(std::istream&);
	double midterm, final;
	std::vector<double> homework;
	virtual Core* clone()const { return new Core(*this); }
private:
	std::string n;
};

#endif // !Core_h