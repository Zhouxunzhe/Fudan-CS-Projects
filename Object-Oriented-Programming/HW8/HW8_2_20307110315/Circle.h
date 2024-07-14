#ifndef Circle_h
#define Circle_h
#include"Shape.h"

class Circle :public Shape {
public:
	Circle() {}
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
