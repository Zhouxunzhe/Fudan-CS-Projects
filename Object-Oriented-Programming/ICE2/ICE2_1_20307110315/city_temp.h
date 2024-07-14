#ifndef GUARD_city_temp_h
#define GUARD_city_temp_h
#include<iostream>
#include<string>
#include<vector>

struct city_temp {
	std::string cityname;
	int temperature;
	double deviate;
};

void read(std::istream&, std::vector<city_temp>&);
double average(std::vector<city_temp>);
void deviate(std::vector<city_temp>&, double);
void print(std::ostream&, std::vector<city_temp>, double);
#endif // !GUARD_city_temp_h
