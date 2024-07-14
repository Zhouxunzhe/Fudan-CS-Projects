#include<stdexcept>
#include"grade.h"
#include"median.h"
#include"Student_info.h"

using std::domain_error; 

double grade(double midterm, double final, double homework) {
	return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}
double grade(double midterm, double final, const Vec<double>& hw) {
	if (hw.size() == 0) throw domain_error("student has done no homework");
	return grade(midterm, final, median(hw.begin(), hw.end()));
}
double Student_info::grade() const {
	return::grade(midterm, final, homework);
}
std::string letter_grade(double grade) {
	if (grade >= 90 && grade <= 100)return "A";
	if (grade > 85 && grade <= 90)return "A-";
	if (grade >= 82 && grade < 85)return "B+";
	if (grade >= 78 && grade < 82)return "B";
	if (grade >= 75 && grade < 78)return "B-";
	if (grade >= 71 && grade < 75)return "C+";
	if (grade >= 66 && grade < 71)return "C";
	if (grade >= 62 && grade < 66)return "C-";
	if (grade >= 60 && grade < 62)return "D";
	if (grade < 60)return "F";
}