#include<iostream>
using std::cout;
using std::cin; using std::endl;

int main() {
	int year, month, date, hundred = 100;
	cout << "Enter the baby's birthday: ";
	cin >> year >> month >> date;
	while (hundred >= 0) {
		switch (month) {
		case 1:hundred -= 31 - date; break;
		case 2:if (year % 400 == 0)
			hundred -= 29 - date;
			  else if (year % 4 == 0 && year % 100 != 0)
			hundred -= 29 - date;
			  else
			hundred -= 28 - date;
			break;
		case 3:hundred -= 31 - date; break;
		case 4:hundred -= 30 - date; break;
		case 5:hundred -= 31 - date; break;
		case 6:hundred -= 30 - date; break;
		case 7:hundred -= 31 - date; break;
		case 8:hundred -= 31 - date; break;
		case 9:hundred -= 30 - date; break;
		case 10:hundred -= 31 - date; break;
		case 11:hundred -= 30 - date; break;
		case 12:hundred -= 31 - date; break;
		}
		hundred -= 1;
		month++;
		date = 1;
		if (month == 13) {
			year++;
			month = 1;
		}
		if (hundred < 0) {
			month--;
			switch (month) {
			case 0:month = 12; year--; date = 31 + hundred; break;
			case 1:date = 31 + hundred; break;
			case 2:if (year % 400 == 0)
				date = 29 + hundred;
				  else if (year % 4 == 0 && year % 100 != 0)
				date = 29 + hundred;
				  else
				date = 28 + hundred;
				break;
			case 3:date = 31 + hundred; break;
			case 4:date = 30 + hundred; break;
			case 5:date = 31 + hundred; break;
			case 6:date = 30 + hundred; break;
			case 7:date = 31 + hundred; break;
			case 8:date = 31 + hundred; break;
			case 9:date = 30 + hundred; break;
			case 10:date = 31 + hundred; break;
			case 11:date = 30 + hundred; break;
			case 12:date = 31 + hundred; break;
			}
		}
		if (date == 0) {
			month--;
			switch (month) {
			case 0:month = 12; year--; date = 31; break;
			case 1:date = 31; break;
			case 2:if (year % 400 == 0)
				date = 29;
				  else if (year % 4 == 0 && year % 100 != 0)
				date = 29;
				  else
				date = 28;
				break;
			case 3:date = 31; break;
			case 4:date = 30; break;
			case 5:date = 31; break;
			case 6:date = 30; break;
			case 7:date = 31; break;
			case 8:date = 31; break;
			case 9:date = 30; break;
			case 10:date = 31; break;
			case 11:date = 30; break;
			case 12:date = 31; break;
			}
		}
	}
	cout << "The baby's 100-day date is: " << year << " " << month << " " << date << endl;
	return 0;
}