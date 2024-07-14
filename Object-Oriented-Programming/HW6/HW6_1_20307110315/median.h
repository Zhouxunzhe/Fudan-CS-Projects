#ifndef GURAD_median_h
#define GUARD_median_h
#include<algorithm>
#include<stdexcept>
#include"Vec.h"


template <class Iter>
struct iter_traits
{
	typedef typename Iter::value_type value_type;
};
template <class T>
struct iter_traits<T*>
{
	typedef T value_type;
};
template <class T>
struct iter_traits<const T*>
{
	typedef T value_type;
};

template<typename T>
typename iter_traits<T>::value_type median(T b, T e) {
	int size = 0, mid;
	T median = b;
	while (median != e) {
		++median;
		size++;
	}
	Vec<typename iter_traits<T>::value_type> tmp;
	for (T i = b; i != e; i++) {
		tmp.push_back(*i);
	}
	if (size == 0) {
		throw std::domain_error("median of an empty vector");
	}
	std::sort(tmp.begin(), tmp.end());
	mid = size / 2;
	return size % 2 == 0 ? (tmp[mid] + tmp[mid - 1]) / 2 : tmp[mid];
}
#endif // !GURAD_median_h