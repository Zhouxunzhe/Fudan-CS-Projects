#pragma warning(disable:4996)
#ifndef GUARD_Game2048_h
#define GUARD_Game2048_h
#include<ctime>
#include<fstream>
#include"Board.h"

class Game2048 {
public:
	Game2048() { begin_time = end_time = time(NULL); move_valid = true; }//构造函数初始化网格
	~Game2048() {
		end_time = time(NULL);
		std::cout << "Game Over" << std::endl;
		std::ofstream file("game2048.csv", std::ofstream::app);
		std::string str = ctime(&begin_time);
		str.erase(str.end() - 1);
		file << str << "," << end_time - begin_time << "," << board.get_score() << std::endl;
	}//析构函数结束游戏
	char get_movement();//获取移动方向并确保为有效字符
	void get_score();//获取本局游戏得分
	void move_merge();//整体移动数字,合并数字并计分
	void generate();//空白网格随机放置一个数
	void output_board();//输出网格

private:
	Board board;
	bool move_valid;
	time_t begin_time, end_time;
};

#endif // !GUARD_Game2048_h
