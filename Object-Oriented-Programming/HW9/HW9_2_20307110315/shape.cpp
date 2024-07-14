#include"shape.h"

using std::istream;

double Circle::pi = 3.14;

double Rectangle::get_area() const {
	return data[0] * data[1];
}
double Rectangle::get_peri() const {
	return (data[0] + data[1]) * 2;
}
double Circle::get_area() const {
	return pi * data[0] * data[0];
}
double Circle::get_peri() const {
	return 2 * pi * data[0];
}
bool Triangle::check() {
	//not a triangle
	double a = data[0], b = data[1], c = data[2];
	if (a + b > c && a + c > b && b + c > a)
		return true;
	else {
		return false;
	}
}
void Triangle::check_output() {
	std::cout << data[0] << ", " << data[1] << ", " << data[2] << " "
		<< "can't make a triangle." << std::endl;
}
double Triangle::get_area() const {
	double a = data[0], b = data[1], c = data[2];
	double p = (a + b + c) / 2;
	//º£Â×¹«Ê½
	double area = 0;
	if ((p * (p - a) * (p - b) * (p - c)) >= 0) {
		area = sqrt(p * (p - a) * (p - b) * (p - c));
	}
	return area;
}
double Triangle::get_peri() const {
	return data[0] + data[1] + data[2];
}
