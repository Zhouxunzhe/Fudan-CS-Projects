#include"Core.h"
#include"Grad.h"
#include<ios>
#include<iomanip>
#include<fstream> 

using std::vector; using std::string; using std::streamsize;
using std::endl; using std::domain_error; using std::cout;
using std::ifstream; using std::ofstream;

int main() {
	ifstream infile("C:\\Users\\ZhouXunZhe\\Desktop\\grades2");
	vector<Core*> students;
	Core* record;
	char ch;
	string::size_type maxlen = 0;
	while (infile >> ch) {
		if (ch == 'U')
			record = new Core;
		else
			record = new Grad;
		record->read(infile);
		maxlen = std::max(maxlen, record->name().size());
		students.push_back(record);
	}
	std::sort(students.begin(), students.end(), compare_core_ptrs);

	for (vector<Core*>::size_type i = 0; i != students.size(); ++i) {
		cout << students[i]->name()
			<< string(maxlen + 1 - students[i]->name().size(), ' ');
		try {
			double final_grade = students[i]->grade();
			streamsize prec = cout.precision();
			cout << std::setprecision(3) << final_grade
				<< std::setprecision(prec) << endl;
		}
		catch (domain_error e) {
			cout << e.what() << endl;
		}
		delete students[i];
	}
	return 0;
}