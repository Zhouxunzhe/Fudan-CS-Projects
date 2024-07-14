#ifndef GUARD_grade_h
#define GUARD_grade_h
#include<vector>
#include"Student_info.h"
inline double grade(double, double, double);
inline double grade(double, double, const std::vector<double>&);
double grade(const Student_info&);
#endif // !GUARD_grade_h
