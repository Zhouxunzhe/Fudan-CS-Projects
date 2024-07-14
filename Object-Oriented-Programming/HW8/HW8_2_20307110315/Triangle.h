#ifndef Triangle_h
#define Triangle_h
#include"Shape.h"

class Triangle :public Shape {
public:
	Triangle() {}
	bool check();
	void check_output();
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