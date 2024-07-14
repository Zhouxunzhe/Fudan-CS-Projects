#include"Student_info.h"
#include"grade.h"
#include"median.h"
#include<algorithm>
#include<iomanip>
#include<ios>
#include<stdexcept>

using std::cin; using std::vector;
using std::string; using std::max;
using std::cout; using std::endl;
using std::streamsize; using std::setprecision;
using std::domain_error;

int main() {
	vector<Student_info> students;
	Student_info record;
	string::size_type maxlen = 0;
	while (record.read(cin)) {
		maxlen = max(maxlen, record.name().size());
		students.push_back(record);
	}
	sort(students.begin(), students.end(), compare);
	for (vector<Student_info>::size_type i = 0;
		i != students.size(); ++i) {
		cout << students[i].name()
			<< string(maxlen + 1 - students[i].name().size(), ' ');
		try {
			double final_grade = students[i].grade();
			streamsize prec = cout.precision();
			cout << setprecision(3) << final_grade
				<< setprecision(prec) << endl;
		}
		catch (domain_error e) {
			cout << e.what() << endl;
		}
	}
	return 0;
}