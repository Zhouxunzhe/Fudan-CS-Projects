#include"Student_info.h"
#include"grade.h"
#include"median.h"
#include<algorithm>
#include<iomanip>
#include<ios>
#include<stdexcept>

using std::vector; using std::ofstream;
using std::string; using std::max;
using std::cout; using std::endl;
using std::streamsize; using std::setprecision;
using std::domain_error; using std::ifstream; 

int main() {
	ifstream infile("C:\\Users\\ZhouXunZhe\\Desktop\\grades");
	vector<Student_info> students;
	Student_info record;
	string::size_type maxlen = 0;
	while (record.read(infile)) {
		maxlen = max(maxlen, record.name().size());
		students.push_back(record);
	}
	sort(students.begin(), students.end(), compare);
	for (vector<Student_info>::size_type i = 0;
		i != students.size(); ++i) {
		cout << students[i].name()
			<< string(maxlen + 1 - students[i].name().size(), ' ');
		try {
			string final_grade = letter_grade(students[i].grade());
			streamsize prec = cout.precision();
			cout << setprecision(3) << final_grade
				<< setprecision(prec) << endl;
		}
		catch (domain_error e) {
			cout << e.what() << endl;
		}
	}
	ofstream outfile("C:\\Users\\ZhouXunZhe\\Desktop\\final");
	for (vector<Student_info>::size_type i = 0;
		i != students.size(); ++i) {
		outfile << students[i].name()
			<< string(maxlen + 1 - students[i].name().size(), ' ');
		try {
			string final_grade = letter_grade(students[i].grade());
			streamsize prec = outfile.precision();
			outfile << setprecision(3) << final_grade
				<< setprecision(prec) << endl;
		}
		catch (domain_error e) {
			outfile << e.what() << endl;
		}
	}
	int a[11] = { 94,92,89,52,95,66,96,66,53,81,53 };
	cout << "the following is the result of testing median function on array" << endl;
	cout << "the random array is: " << endl;
	for (int i = 0; i < 11; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
	cout << "the median of the array is: " << median(a, a + 11) << endl;
	cout << "after median function called, the array is unchanged:" << endl;
	for (int i = 0; i < 11; i++) {
		cout << a[i] << " ";
	}
	return 0;
}