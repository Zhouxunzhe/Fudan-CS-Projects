#include<iostream>
#include<vector>

using std::cin; using std::vector; 
using std::cout; using std::endl;

vector<int> form(int n) {
	vector<int> square;
	int number = 1;
	if (n == 1) {
		square.push_back(1);
		return square;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n + 2 * (n - 1); j++) {
			square.push_back(0);
		}
	}
	for (int i = n - 1; i < n + 2 * (n - 1); i++) {
		int tmp = i;
		int is_reverse = 0;
		if ((i - n + 1) % 2 == 1) {//reverse
			tmp = i + (n - 1) * (n + 2 * (n - 1) - 1);
			is_reverse = 1;
		}
		if (is_reverse == 0) {
			int j = tmp;
			for ( ; j <= tmp + (n - 1) * (n + 2 * (n - 1) - 1); j += n + 2 * (n - 1) - 1) {
				if (j % (n + 2 * (n - 1)) >= n - 1 && j % (n + 2 * (n - 1)) < 2 * n - 1) {
					square[j] = number;
					number++;
				}
			}
		}
		if (is_reverse == 1) {
			int j = tmp;
			for ( ; j >= i; j -= n + 2 * (n - 1) - 1) {
				if (j % (n + 2 * (n - 1)) >= n - 1 && j % (n + 2 * (n - 1)) < 2 * n - 1) {
					square[j] = number;
					number++;
				}
			}
		}
	}
	return square;
}
void output(vector<int> square, int n) {
	int sig = 0;
	if (n > 3) {
		if (n < 10) {
			sig = 1;
		}
		else {
			sig = 2;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = n - 1; j < 2 * n - 1; j++) {
			if (sig == 1 && square[i * (n + 2 * (n - 1)) + j] < 10) {
				cout << " " << square[i * (n + 2 * (n - 1)) + j] << " ";
			}
			else if (sig == 2 && square[i * (n + 2 * (n - 1)) + j] < 10) {
				cout << "  " << square[i * (n + 2 * (n - 1)) + j] << " ";
			}
			else if (sig == 2 && square[i * (n + 2 * (n - 1)) + j] < 100
							&& square[i * (n + 2 * (n - 1)) + j] > 9) {
				cout << " " << square[i * (n + 2 * (n - 1)) + j] << " ";
			}
			else cout << square[i * (n + 2 * (n - 1)) + j] << " ";
		}
		cout << endl;
	}
}

int main() {
	int n;
	cout << "Enter a positive integer: ";
	cin >> n;
	output(form(n), n);
	return 0;
}