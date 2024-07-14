#include"median.h"
#include<algorithm>
#include<stdexcept>

using std::domain_error; using std::sort; 
using std::vector; using std::istream;

double median(vector<double> vec) {
	typedef vector<double>::size_type lis_sz;
	lis_sz size = vec.size();
	if (size == 0) {
		throw domain_error("median of an empty vector");
	}
	sort(vec.begin(), vec.end());
	lis_sz mid = size / 2;
	return size % 2 == 0 ? (vec[mid] + vec[mid - 1]) / 2 : vec[mid];
}
istream& read_hw(istream& in, vector<double>& hw) {
	if (in) {
		hw.clear();
		double x;
		while (in >> x)
			hw.push_back(x);
		in.clear();
	}
	return in;
}