#include"handle.h"

std::istream& Handle::read(std::istream& is) {
	delete sp;
	char ch;
	is >> ch;
	if (ch == 'C')
		sp = new Circle(is);
	else if (ch == 'T')
		sp = new Triangle(is);
	else if (ch == 'R')
		sp = new Rectangle(is);
	else
		sp = new Shape(is);
	return is;
}
Handle::Handle(const Handle& h) :sp(0) {
	if (h.sp)sp = h.sp->clone();
}
Handle& Handle::operator=(const Handle& h) {
	if (&h != this) {
		delete sp;
		if (h.sp)
			sp = h.sp->clone();
		else
			sp = 0;
	}
	return *this;
}