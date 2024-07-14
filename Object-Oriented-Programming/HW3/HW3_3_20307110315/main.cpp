#include"head.h"
#include<ios>
#include<iomanip>
#include<stdlib.h>
#include<time.h>
#include<list>
#include<string>
#define N 10
#define LEN 5
#define MAX 100
using std::cout; using std::list;
using std::string; using std::endl;
using std::streamsize; using std::setprecision;

int main() {
	list<int> list_int;
	list<double> list_double;
	list<string> list_string;
	double width = 5.0;
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++) {
		list_int.push_back(rand() % MAX);
		list_double.push_back(width * rand() / double(RAND_MAX));
		int length = rand() % LEN + 1;
		string s = "";
		for (int i = 0; i < length; i++) {
			s += 'a' + rand() % 26;
		}
		list_string.push_back(s);
	}

	cout << "the integer list:" << endl;
	output(list_int.begin(), list_int.end());
	cout << "the sum of the integer list: " << 
		accumulate(list_int.begin(), list_int.end(), 0) << endl;
	cout << "the max element in the integer list: " <<
		*max_element(list_int.begin(), list_int.end()) << endl;
	cout << "the min element in the integer list: " <<
		*min_element(list_int.begin(), list_int.end()) << endl;
	cout << endl;

	streamsize prec = cout.precision();
	cout << "the double list:" << setprecision(6) << endl;
	output(list_double.begin(), list_double.end());
	cout << "the sum of the double list: " <<
		accumulate(list_double.begin(), list_double.end(), 0.0) << endl;
	cout << "the max element in the double list: " <<
		*max_element(list_double.begin(), list_double.end()) << endl;
	cout << "the min element in the double list: " <<
		*min_element(list_double.begin(), list_double.end()) << endl;
	cout << setprecision(prec) << endl;
	
	cout << "the string list:" << endl;
	output(list_string.begin(), list_string.end());
	cout << "the concatenation of the string list: " <<
		accumulate(++list_string.begin(), list_string.end(), *list_string.begin()) << endl;
	cout << "the max string in the string list: " <<
		*max_element(list_string.begin(), list_string.end()) << endl;
	cout << "the min string in the string list: " <<
		*min_element(list_string.begin(), list_string.end()) << endl;
	return 0;
}