#include"Circle.h"


double Circle::get_area() const {
	return 3.14 * data[0] * data[0];
}
double Circle::get_peri() const {
	return 2 * 3.14 * data[0];
}