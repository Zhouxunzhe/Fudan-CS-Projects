#include"Core.h"
#include"grade.h"
#include"median.h"

using std::string; using std::istream;

string Core::name() const { return n; }

double Core::grade() const {
	return::grade(midterm, final, homework);
}

istream& Core::read_common(istream& in) {
	in >> n >> midterm >> final;
	return in;
}

istream& Core::read(istream& in) {
	read_common(in);
	read_hw(in, homework);
	return in;
}