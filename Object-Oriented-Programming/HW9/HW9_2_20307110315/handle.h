#ifndef handle_h
#define handle_h
#include"shape.h"

class Handle {
public:
	Handle() :sp(0) {}
	Handle(std::istream& is) :sp(0) { read(is); }
	Handle(const Handle&);
	Handle& operator=(const Handle&);
	~Handle() { delete sp; }

	std::istream& read(std::istream&);
	bool check() const {
		if (sp)return sp->check();
		else throw std::runtime_error("uninitialized shape");
	}
	void check_output() const {
		if (sp)return sp->check_output();
		else throw std::runtime_error("uninitialized shape");
	}
	double get_area() const {
		if (sp)return sp->get_area();
		else throw std::runtime_error("uninitialized shape");
	}
	double get_peri() const {
		if (sp)return sp->get_peri();
		else throw std::runtime_error("uninitialized shape");
	}
	static bool compare(const Handle& h1, const Handle& h2) {
		return h1.get_area() < h2.get_area();
	}
private: 
	Shape* sp;
};
#endif // !handle_h