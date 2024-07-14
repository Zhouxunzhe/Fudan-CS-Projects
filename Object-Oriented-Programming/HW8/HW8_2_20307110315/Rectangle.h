#ifndef Rectangle_h
#define Rectangle_h
#include"Shape.h"

class Rectangle :public Shape {
public:
	Rectangle() {}
	double get_area() const;
	double get_peri() const;
	std::istream& read_data(std::istream& is) {
		if (is) {
			data.clear();
			double x;
			while (is >> x)
				data.push_back(x);
			is.clear();
		}
		return is;
	}
};

#endif // !Circle_h