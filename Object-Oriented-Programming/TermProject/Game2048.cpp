#include"Game2048.h"

using std::cin; using std::cout;
using std::endl; 

void Game2048::move_merge() {
	switch (get_movement()) {
		case 'l': {
			board.move();
			board.merge();
			board.move();
			break;
		}
		case 'u': {
			//ֻ�轫����ת�ú������Ƽ�Ϊԭ������������
			board.trans();
			board.move();
			board.merge();
			board.move();
			board.trans();
			break;
		}
		case 'r': {
			//ֻ�轫������ˮƽ��ת�������Ƽ�Ϊԭ������������
			board.reverse();
			board.move();
			board.merge();
			board.move();
			board.reverse();
			break;
		}
		case 'd': {
			//�������ش�ֱ��ת��ת�ú������Ƽ�Ϊԭ������������
			board.updown();
			board.trans();
			board.move();
			board.merge();
			board.move();
			board.trans();
			board.updown();
			break;
		}
		case 'q': {
			exit(0); 
		}
	}
	if (board.is_full() && !board.is_merge()) {
		exit(0);
	}
}
char Game2048::get_movement() {
	char move;
	cin >> move;
	if (move == 'l' ||
		move == 'r' ||
		move == 'u' ||
		move == 'd' ||
		move == 'q') {
		move_valid = true;
		return move;
	}
	move_valid = false;
	return '\0';
}
void Game2048::get_score() {
	if (move_valid) {
		cout << "Your score: " << board.get_score() << endl;
		move_valid = false;
	}
}
void Game2048::generate() {
	if (move_valid) {
		board.generate();
	}
}
void Game2048::output_board() {
	if (move_valid) {
		board.output_board();
	}
}