#include<iostream>
#include<vector>
#include<algorithm>
#include<stdexcept>
#include<iomanip>
#include<ios>
#include"city_temp.h"
using namespace std;

bool cmp(const city_temp& x, const city_temp& y) {
    return x.deviate < y.deviate;
}
void read(istream& cin,vector<city_temp>& cities) {
    if (cin) {
        cities.clear();
        city_temp ct;
        while (cin >> ct.cityname >> ct.temperature) {
            ct.deviate = 0;
            cities.push_back(ct);
        }
    }
}
double average(vector<city_temp> cities) {
    int sz = cities.size(); int sum = 0;
    double avg;
    if (sz == 0) throw domain_error("No city and temperature input");
    else {
        for (int i = 0; i < sz; i++) {
            sum += cities[i].temperature;
        }
    }
    return avg = (double)sum / (double)sz;
}
void deviate(vector<city_temp>& cities, double avg) {
    for (int i = 0; i < cities.size(); i++) {
        cities[i].deviate = cities[i].temperature > avg ?
            cities[i].temperature - avg :
            avg - cities[i].temperature;
    }
    sort(cities.begin(), cities.end(), cmp);
}
void print(ostream& cout, vector<city_temp> cities, double avg) {
    int end = cities.size();
    streamsize prec = cout.precision();
    cout << "The city with the largest deviation from the average temperature is : " << cities[end - 1].cityname << endl;
    cout << setprecision(3) << "It's temperature is: " << cities[end - 1].temperature << setprecision(prec) << endl;
    cout << setprecision(3) << "The deviation is : " << cities[end - 1].deviate << setprecision(prec) << endl;
    cout << "The city with the smallest deviation from the average temperature is : " << cities[0].cityname << endl;
    cout << setprecision(3) << "It's temperature is: " << cities[0].temperature << setprecision(prec) << endl;
    cout << setprecision(3) << "The deviation is : " << cities[0].deviate << setprecision(prec) << setprecision(prec) << endl;
}

int main()
{
    vector<city_temp> cities;
    cout << "Enter city names and temperatures: " << endl;
    read(cin, cities);  //读入城市和温度数据
    try {
        double avg = average(cities);  //求平均温度(含抛出异常)
        deviate(cities, avg);  //求温度偏差并按偏差排序
        print(cout, cities, avg);  //按要求输出
    }
    catch (domain_error e) {
        cout << e.what() << endl;
    }
    return 0;
}