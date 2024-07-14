#ifndef GUARD_head_h
#define GUARD_head_h
#include<iostream>

template<class It, class T>
T accumulate(It b, It e, T t) {
	while (b != e) {
		t += *b;
		++b;
	}
	return t;
}
template<class It>
It max_element(It b, It e) {
	It t = b;
	while (b != e) {
		if (*t < *b)
			*t = *b;
		++b;
	}
	return t;
}
template<class It>
It min_element(It b, It e) {
	It t = b;
	while (b != e) {
		if (*t > *b)
			*t = *b;
		++b;
	}
	return t;
}
template<class It>
void output(It b, It e) {
	while(b!=e){
		std::cout << *b << " ";
		++b;
	}
	std::cout << std::endl;
}

#endif // !GUARD_head_h
