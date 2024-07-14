#ifndef GUARD_grade_h
#define GUARD_grade_h
#include"Vec.h"
#include<string>

inline double grade(double, double, double);
inline double grade(double, double, const Vec<double>&);
std::string letter_grade(double);
#endif // !GUARD_grade_h