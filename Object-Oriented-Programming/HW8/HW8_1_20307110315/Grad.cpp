#include"Grad.h"
#include"Core.h"
#include"median.h"

using std::istream;

istream& Grad::read(istream& in) {
	read_common(in);
	in >> thesis;
	read_hw(in, Core::homework);
	return in;
}

double Grad::grade() const {
	return std::min(Core::grade(), thesis);
}

bool compare(const Core& c1, const Core& c2) {
	return c1.name() < c2.name();
}
bool compare_grade(const Core& c1, const Core& c2) {
	return c1.grade() < c2.grade();
}
bool compare_core_ptrs(const Core* cp1, const Core* cp2) {
	return compare(*cp1, *cp2);
}