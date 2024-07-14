#include"Triangle.h"

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
	//���׹�ʽ
	double area = 0;
	if ((p * (p - a) * (p - b) * (p - c)) >= 0) {
		area = sqrt(p * (p - a) * (p - b) * (p - c));
	}
	return area;
}
double Triangle::get_peri() const {
	return data[0] + data[1] + data[2];
}