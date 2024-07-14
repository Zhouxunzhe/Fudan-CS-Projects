#include"Board.h"
#include"Game2048.h"
using std::cout; using std::endl;

Game2048 game;//定义为全局变量，否则exit（0）不会调用析构函数

int main() {
	cout << "Welcome to the game of 2048!" << endl;
	while (1) {
		game.output_board();
		game.get_score();
		cout << " (↑:u)(↓:d)(←:1)(→:r)(quit:q): ";
		game.move_merge();
		game.generate();
	}
}