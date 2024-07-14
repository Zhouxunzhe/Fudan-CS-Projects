#include"Student_info.h"

using std::istream; using std::vector;

bool compare(const Student_info& x, const Student_info& y) {
	return x.name() < y.name();
}
istream& read_hw(istream& is, vector<double>& hw) {
	if (is) {
		hw.clear();
		double x;
		while (is >> x)
			hw.push_back(x);
		is.clear();
	}
	return is;
}
istream& Student_info::read(istream& in) {
	in >> n >> midterm >> final;
	read_hw(in, homework);
	return in;
}