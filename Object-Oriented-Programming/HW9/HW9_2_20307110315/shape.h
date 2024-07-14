#pragma once
#ifndef shape_h
#define shape_h
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
	virtual std::istream& readinfo(std::istream& is) {
		return is;
	}
	friend class Handle;
protected:
	std::istream& read(std::istream& is) {
		if (is) {
			data.clear();
			double x;
			while (is >> x)
				data.push_back(x);
			is.clear();
		}
		return is;
	}
	std::vector<double> data;
	virtual Shape* clone() const { return new Shape(*this); }
};

class Rectangle :public Shape {
public:
	Rectangle() {}
	Rectangle(std::istream& is) { read(is); }
	double get_area() const;
	double get_peri() const;
	std::istream& readinfo(std::istream& is) {
		return read(is);
	}
protected:
	Rectangle* clone() const { return new Rectangle(*this); }
};

class Circle :public Shape {
public:
	Circle() {}
	Circle(std::istream& is) { read(is); }
	double get_area() const;
	double get_peri() const;
	std::istream& readinfo(std::istream& is) {
		return read(is);
	}
protected:
	Circle* clone() const { return new Circle(*this); }
private:
	static double pi;
};

class Triangle :public Shape {
public:
	Triangle() {}
	Triangle(std::istream& is) { read(is); }
	bool check();
	void check_output();
	double get_area() const;
	double get_peri() const;
	std::istream& readinfo(std::istream& is) {
		return read(is);
	}
protected:
	Triangle* clone() const { return new Triangle(*this); }
};

#endif // !shape_h