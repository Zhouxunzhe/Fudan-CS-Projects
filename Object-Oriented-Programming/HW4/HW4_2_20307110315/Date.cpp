#include"Date.h"

using std::domain_error;

int a[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int b[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

bool Date::is_leap() {
    if (year % 400 == 0)
        return true;
    else
    {
        if (year % 4 == 0 && year % 100 != 0)
            return true;
    }
    return false;
}
void Date::is_valid() {
    if (month > 12 || month < 1) {
        throw domain_error("invalid month");
    }
    if (is_leap()) {
        if (day < 1 || day > a[month - 1]) {
            throw domain_error("invalid day");
        }
    }
    else {
        if (day < 1 || day > b[month - 1]) {
            throw domain_error("invalid day");
        }
    }
}
void Date::add_year() { 
    year += 1;
    if (!is_leap() && month == 2 && day == 29) {
        day = 28;
    }
}
void Date::add_month() { 
    month += 1; 
    if (month == 13) {
        month = 1;
        year += 1;
    }
    else {
        if (is_leap()) { //ÈòÄê
            if (day > a[month - 1]) { 
                day = a[month - 1]; 
            }
        }
        else {
            if (day > b[month - 1]) {
                day = b[month - 1];
            }
        }
    }
}
void Date::add_day() { 
    day += 1; 
    if (is_leap()) {
        if (day > a[month - 1]) {
            month += 1;
            day = 1;
        }
    }
    else {
        if (day > b[month - 1]) {
            month += 1;
            day = 1;
        }
    }
    if (month == 13) {
        year += 1;
        month = 1;
    }
}
int interval(Date d1, Date d2) {
    int cnt = 0;
    if (d1.get_year() < d2.get_year()) {
        while (d1.get_year() != d2.get_year() ||
               d1.get_month() != d2.get_month() ||
               d1.get_day() != d2.get_day()) {
            d1.add_day();
            cnt++;
        }
    }
    else if(d2.get_year() < d1.get_year()) {
        while (d1.get_year() != d2.get_year() ||
            d1.get_month() != d2.get_month() ||
            d1.get_day() != d2.get_day()) {
            d2.add_day();
            cnt++;
        }
    }
    else if (d1.get_year() == d2.get_year()) {
        if (d1.get_month() < d2.get_month()) {
            while (d1.get_month() != d2.get_month() ||
                d1.get_day() != d2.get_day()) {
                d1.add_day();
                cnt++;
            }
        }
        else if (d2.get_month() < d1.get_month()) {
            while (d1.get_month() != d2.get_month() ||
                d1.get_day() != d2.get_day()) {
                d2.add_day();
                cnt++;
            }
        }
        else if (d1.get_month() == d2.get_month()) {
            if (d1.get_day() < d2.get_day()) {
                while (d1.get_day() != d2.get_day()) {
                    d1.add_day();
                    cnt++;
                }
            }
            else if (d2.get_day() < d1.get_day()) {
                while (d1.get_day() != d2.get_day()) {
                    d2.add_day();
                    cnt++;
                }
            }
            else if (d1.get_day() == d2.get_day()) {
                cnt = 0;
            }
        }
    }
    return cnt;
}