#include"Rectangle.h"


double Rectangle::get_area() const {
	return data[0] * data[1];
}
double Rectangle::get_peri() const {
	return (data[0] + data[1]) * 2;
}