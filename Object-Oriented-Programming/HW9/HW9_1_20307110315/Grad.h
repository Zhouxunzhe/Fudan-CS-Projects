#pragma once
#ifndef Grad_h
#define Grad_h
#include"Core.h"

class Grad :public Core {
public:
	Grad() :thesis(0) { }
	Grad(std::istream& is) { read(is); }
	double grade()const;
	std::istream& read(std::istream&);
protected:
	Grad* clone()const { return new Grad(*this); }
private:
	double thesis;
};
bool compare(const Core&, const Core&);
bool compare_grade(const Core&, const Core&);
bool compare_core_ptrs(const Core*, const Core*);
#endif // ! Grad_h
