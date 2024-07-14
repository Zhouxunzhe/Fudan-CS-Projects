#include<iostream>
using std::cout;
using std::cin; using std::endl;

int main() {
	int sq, rectL, rectW, tri;
	cout << "Enter the side length of a square: ";
	cin >> sq;
	for (int i = 0; i < sq; i++) {
		for (int j = 0; j < sq; j++) {
			cout << "* ";
		}
		cout << endl;
	}
	cout<< "Enter the length and width of a rectangle: ";
	cin >> rectL >> rectW;
	for (int i = 0; i < rectW; i++) {
		for (int j = 0; j < rectL; j++) {
			cout << "* ";
		}
		cout << endl;
	}
	cout<< "Enter the base length of an isosceles triangle(odd number): ";
	cin >> tri;
	for (int i = 0; i < tri / 2 + 1; i++) {
		for (int j = 0; j < 2 * (tri / 2 - i); j++) {
			cout << " ";
		}
		for (int k = 0; k < 2 * i + 1; k++) {
			cout << "* ";
		}
		cout << endl;
	}
	return 0;
}