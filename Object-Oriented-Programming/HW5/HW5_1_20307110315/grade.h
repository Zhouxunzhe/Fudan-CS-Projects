#ifndef GUARD_grade_h
#define GUARD_grade_h
#include<vector>
#include<string>

inline double grade(double, double, double);
inline double grade(double, double, const std::vector<double>&);
std::string letter_grade(double);
#endif // !GUARD_grade_h