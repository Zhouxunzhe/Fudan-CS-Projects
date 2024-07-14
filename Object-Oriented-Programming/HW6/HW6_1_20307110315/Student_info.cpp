#include"Student_info.h"

using std::ifstream;

bool compare(const Student_info& x, const Student_info& y) {
	return x.name() < y.name();
}
ifstream& read_hw(ifstream& is, Vec<double>& hw) {
	if (is) {
		hw.clear();
		double x;
		while (is >> x)
			hw.push_back(x);
		is.clear();
	}
	return is;
}
ifstream& Student_info::read(ifstream& in) {
	in >> n >> midterm >> final;
	read_hw(in, homework);
	return in;
}