#include"Shape.h"

using std::istream;

istream& Shape::read(istream& is) {
	if (is) {
		data.clear();
		double x;
		while (is >> x)
			data.push_back(x);
		is.clear();
	}
	return is;
}