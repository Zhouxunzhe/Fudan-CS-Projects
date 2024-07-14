#pragma once
#ifndef GUARD_Str_h
#define GUARD_Str_h
#include"Vec.h"

class Str {
public:
	typedef Vec<char>::size_type size_type;
	size_type size()const { return data.size(); }

	Str() { }
	Str(size_type n, char c) :data(n, c) { }
	Str(const char* cp) {
		std::copy(cp, cp + std::strlen(cp), std::back_inserter(data));
	}
	template<class In>Str(In b, In e) {
		std::copy(b, e, std::back_inserter(data));
	}

	char& operator[](size_type i) { return data[i]; }
	const char& operator[](size_type i)const { return data[i]; }
	friend std::istream& operator>>(std::istream& is, Str& s);
	Str& operator+=(const Str& s);
private:
	Vec<char>data;
};

std::ostream& operator<<(std::ostream& os, const Str& s) {
	for (Str::size_type i = 0; i != s.size(); ++i)
		os << s[i];
	return os;
}
std::istream& operator>>(std::istream& is, Str& s) {
	s.data.clear();
	char c;
	while (is.get(c) && isspace(c));
	if (is) {
		do s.data.push_back(c);
		while (is.get(c) && !isspace(c));
		if (is)
			is.unget();
	}
	return is;
}
Str& Str::operator+=(const Str& s) {
	std::copy(s.data.begin(), s.data.end(),
		std::back_inserter(data));
	return *this;
}
Str operator+(const Str& s, const Str& t) {
	Str r = s;
	r += t;
	return r;
}
#endif // !GUARD_Str_h
