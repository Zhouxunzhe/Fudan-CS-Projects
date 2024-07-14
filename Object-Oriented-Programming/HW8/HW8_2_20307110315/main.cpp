#include"Shape.h"
#include"Circle.h"
#include"Rectangle.h"
#include"Triangle.h"
#include<fstream>
#include<iomanip>
#include<algorithm>

using std::cout; using std::ifstream;
using std::vector; using std::string;
using std::endl; using std::domain_error;
using std::ofstream; using std::streamsize;

bool compare(const Shape& s1, const Shape& s2) {
	return s1.get_area() < s2.get_area();
}
bool compare_shape_ptr(const Shape* sp1, const Shape* sp2) {
	return compare(*sp1, *sp2);
}

int main() {
	ifstream infile("C:\\Users\\ZhouXunZhe\\Desktop\\shapes");
	ofstream outfile("C:\\Users\\ZhouXunZhe\\Desktop\\result.csv");
	vector<Shape*> shapes;
	Shape* record;
	char ch;
	string::size_type maxlen = 0;
	streamsize prec = cout.precision();
	while (infile >> ch) {
		if (ch == 'C')
			record = new Circle;
		else if (ch == 'T')
			record = new Triangle;
		else if (ch == 'R')
			record = new Rectangle;
		else
			record = new Shape;
		record->read_data(infile);
		shapes.push_back(record);
	}
	std::sort(shapes.begin(), shapes.end(), compare_shape_ptr);
	for (vector<Shape*>::size_type i = 0; i != shapes.size(); ++i) {
		if(!shapes[i]->check()){
			shapes[i]->check_output();
		}
	}
	cout << endl << "Area    Perimeter" << endl;
	outfile << "Area,Perimeter" << endl;
	for (vector<Shape*>::size_type i = 0; i != shapes.size(); ++i) {
		if (shapes[i]->check()) {
			cout << std::setprecision(3) << std::setw(8) << std::left
				<< shapes[i]->get_area() << std::setw(8) << std::left
				<< shapes[i]->get_peri() << std::setprecision(prec) << endl;
			outfile << std::setprecision(3) << shapes[i]->get_area() <<","
				<< shapes[i]->get_peri() << std::setprecision(prec) << endl;
		}
		delete shapes[i];
	}
	return 0;
}