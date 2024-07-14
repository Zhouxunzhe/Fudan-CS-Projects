#include"Board.h"

using std::cout; using std::endl;
using std::string; using std::vector;

Board::Board() {
	srand((unsigned)time(NULL));
	score = 0; 
	memset(board, 0, sizeof(board)); 
	int x1 = rand() % 4; int y1 = rand() % 4;
	board[x1][y1] = 2;
	vector<int*>vptr;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				int* p = &board[i][j];
				vptr.push_back(p);
			}
		}
	}
	int random = rand() % 15;
	*vptr[random] = 2;
}

void Board::output_board() {//最大的可达成的数为2^17=131072
	for (int row = 0; row < (4 + 5); row++) {
		if (row % 2 == 0) {
			cout << "|------|------|------|------|" << endl;
		}
		else {
			for (int col = 0; col < 4; col++) {
				cout << "|";
				int num = board[row / 2][col];
				if (num != 0) {//如果数字非零则居中输出
					string num2str = std::to_string(num);
					int before = (6 - num2str.size()) / 2;
					int after = 6 - before - num2str.size();
					string bstr(before, ' ');
					string astr(after, ' ');
					string str = bstr + num2str + astr;
					cout << str;
				}
				else {//如果数字为零则不输出
					string spaces(6, ' ');
					cout << spaces;
				}
			}
			cout << "|" << endl;
		}
	}
}
void Board::generate() {
	srand((unsigned)time(NULL));
	vector<int*>vptr;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				int* p = &board[i][j];
				vptr.push_back(p);
			}
		}
	}
	int len = vptr.size();
	if (len != 0) {
		int random = rand() % len;
		int toworfour = rand() % 5;
		if (toworfour == 0) *vptr[random] = 4;
		else *vptr[random] = 2;
	}
}
void Board::move() {
	//只需实现向左的移动即可
	for (int col = 1; col < 4; col++) {//对每次移动只需判断右边三列能否移动
		for (int row = 0; row < 4; row++) {
			while (board[row][col - 1] == 0 && board[row][col] != 0 && col > 0) {
				board[row][col - 1] = board[row][col];
				board[row][col] = 0;
				col--;
			}
		}
	}
}
void Board::merge() {
	for (int col = 0; col < 3; col++) {
		for (int row = 0; row < 4; row++) {
			if (board[row][col] == board[row][col + 1]) {
				board[row][col] *= 2;
				score += board[row][col];
				board[row][col + 1] = 0;
			}
		}
	}
}
bool Board::is_full() {
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) cnt++;
		}
	}
	if (cnt == 0) return true;
	return false;
}
bool Board::is_merge() {
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 3; col++) {
			if (board[row][col] == board[row][col + 1])
				return true;
		}
	}
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 4; col++) {
			if (board[row][col] == board[row + 1][col])
				return true;
		}
	}
	return false;
}
void Board::trans() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j <= i; j++) {
			int tmp;
			tmp = board[i][j];
			board[i][j] = board[j][i];
			board[j][i] = tmp;
		}
	}
}

void Board::reverse() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			int tmp;
			tmp = board[i][j];
			board[i][j] = board[i][3 - j];
			board[i][3 - j] = tmp;
		}
	}
}
void Board::updown() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			int tmp;
			tmp = board[i][j];
			board[i][j] = board[3 - i][j];
			board[3 - i][j] = tmp;
		}
	}
}