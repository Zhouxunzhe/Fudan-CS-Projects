#include"Shape.h"
#include"handle.h"
#include<fstream>
#include<iomanip>
#include<algorithm>

using std::cout; using std::ifstream;
using std::vector; using std::string;
using std::endl; using std::domain_error;
using std::ofstream; using std::streamsize;

void readinfo(std::istream& is, vector<Handle>& shapes) {
	Handle record;
	while (record.read(is)) {
		shapes.push_back(record);
	}
}
void cal_output(const vector<Handle>& shapes) {
	streamsize prec = cout.precision();
	ofstream outfile("C:\\Users\\ZhouXunZhe\\Desktop\\result.csv");
	for (vector<Handle>::size_type i = 0; i != shapes.size(); ++i) {
		if (!shapes[i].check()) {
			shapes[i].check_output();
		}
	}
	cout << endl << "Area    Perimeter" << endl;
	outfile << "Area,Perimeter" << endl;
	for (vector<Handle>::size_type i = 0; i != shapes.size(); ++i) {
		if (shapes[i].check()) {
			cout << std::setprecision(3) << std::setw(8) << std::left
				<< shapes[i].get_area() << std::setw(8) << std::left
				<< shapes[i].get_peri() << std::setprecision(prec) << endl;
			outfile << std::setprecision(3) << shapes[i].get_area() << ","
				<< shapes[i].get_peri() << std::setprecision(prec) << endl;
		}
	}
}

int main() {
	vector<Handle> shapes;
	ifstream infile("C:\\Users\\ZhouXunZhe\\Desktop\\shapes");
	readinfo(infile, shapes);
	std::sort(shapes.begin(), shapes.end(), Handle::compare);
	cal_output(shapes);

	return 0;
}