#ifndef GURAD_median_h
#define GUARD_median_h
#include<algorithm>
#include<stdexcept>
#include<vector>

template<typename T>
typename T::value_type median(T b, T e) {
	int size = 0, mid;
	T median = b;
	while (median != e) {
		++median;
		size++;
	}
	std::vector<typename T::value_type> tmp;
	for (T i = b; i != e; i++) {
		tmp.push_back(*i);
	}
	if (size == 0) {
		throw std::domain_error("median of an empty vector");
	}
	sort(tmp.begin(), tmp.end());
	mid = size / 2;
	return size % 2 == 0 ? (tmp[mid] + tmp[mid - 1]) / 2 : tmp[mid];
}
template<typename T>
T median(T* b, T* e) {
	int size = 0, mid;
	T* median = b;
	while (median != e) {
		++median;
		size++;
	}
	std::vector<T> tmp;
	for (int i = 0; i < size; i++) {
		tmp.push_back(b[i]);
	}
	if (size == 0) {
		throw std::domain_error("median of an empty vector");
	}
	sort(tmp.begin(), tmp.end());
	mid = size / 2;
	return size % 2 == 0 ? (tmp[mid] + tmp[mid - 1]) / 2 : tmp[mid];
}
#endif // !GURAD_median_h