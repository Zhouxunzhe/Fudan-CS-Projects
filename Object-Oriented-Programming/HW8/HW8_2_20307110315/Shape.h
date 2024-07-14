#ifndef Shape_h
#define Shape_h
#include<string>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<cmath>

class Shape {
public:
	Shape() {}
	Shape(std::istream& is) { read(is); }
	virtual ~Shape() {}
	virtual bool check() { return true; }
	virtual void check_output() {}
	virtual double get_area() const { return 0; }
	virtual double get_peri() const { return 0; }
	virtual std::istream& read_data(std::istream& is) {
		return is;
	}
protected:
	std::istream& read(std::istream&);
	std::vector<double> data;
};

#endif // !Shape_h
