#ifndef GUARD_Board_h
#define GUARD_Board_h
#include<string>
#include<vector>
#include<time.h>
#include<stdlib.h>
#include<iostream>

class Board {
public:
	Board();//构造函数初始化网格
	int get_score() { return score; }//获取得分
	void output_board();//输出网格
	void generate();//空白网格随机放置一个数
	void move();//根据输入整体移动数字
	void merge();//合并数字并计分
	bool is_full();//判断网格是否填满
	bool is_merge();//判断网格是否还可以合并
	void trans();//将网格转置
	void reverse();//将网格沿水平翻转
	void updown();//将网格沿垂直翻转

private:
	int score;//数据成员：分数
	int board[4][4];//数据成员：网格
};

#endif // !GUARD_Game2048_h