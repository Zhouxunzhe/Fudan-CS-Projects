#include<algorithm>
#include<iomanip>
#include<ios>
#include<stdexcept>
#include<string>
#include<list>
#include<vector>
#include"grade.h"
#include"Student_info.h"

using std::cin; using std::setprecision;
using std::cout; using std::sort;
using std::domain_error; using std::streamsize;
using std::endl; using std::string;
using std::max; using std::vector;
using std::list;

int main() {
	list<Student_info> students, fail;
	Student_info record;
	string::size_type maxlen = 0;
	while (read(cin, record)) {
		maxlen = max(maxlen, record.name.size());
		students.push_back(record);
	}
	list<Student_info>::iterator iter = students.begin();
	while(iter!=students.end()){
		try { iter->grade = grade(*iter); }
		catch (domain_error e) {
			cout << iter->name
				<< string(maxlen + 1 - iter->name.size(), ' ');
			cout << e.what() << endl;
			iter->grade = 101;
		}
		++iter;
	}
	fail = extract_fails(students);
	students.sort(compare);
	fail.sort(compare);
	cout << "passing student records: " << endl;
	iter = students.begin();
	while (iter != students.end()) {
		if (iter->grade <= 100) {
			cout << iter->name
				<< string(maxlen + 1 - iter->name.size(), ' ');
			double final_grade = iter->grade;
			streamsize prec = cout.precision();
			cout << setprecision(3) << final_grade
				<< setprecision(prec);
			cout << endl;
		}
		++iter;
	}
	cout << "failing student records: " << endl;
	iter = fail.begin();
	while (iter != fail.end()) {
		cout << iter->name
			<< string(maxlen + 1 - iter->name.size(), ' ');
		double final_grade = iter->grade;
		streamsize prec = cout.precision();
		cout << setprecision(3) << final_grade
			<< setprecision(prec);
		cout << endl;
		++iter;
	}
	return 0;
}