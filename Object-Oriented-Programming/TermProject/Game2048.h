#pragma warning(disable:4996)
#ifndef GUARD_Game2048_h
#define GUARD_Game2048_h
#include<ctime>
#include<fstream>
#include"Board.h"

class Game2048 {
public:
	Game2048() { begin_time = end_time = time(NULL); move_valid = true; }//���캯����ʼ������
	~Game2048() {
		end_time = time(NULL);
		std::cout << "Game Over" << std::endl;
		std::ofstream file("game2048.csv", std::ofstream::app);
		std::string str = ctime(&begin_time);
		str.erase(str.end() - 1);
		file << str << "," << end_time - begin_time << "," << board.get_score() << std::endl;
	}//��������������Ϸ
	char get_movement();//��ȡ�ƶ�����ȷ��Ϊ��Ч�ַ�
	void get_score();//��ȡ������Ϸ�÷�
	void move_merge();//�����ƶ�����,�ϲ����ֲ��Ʒ�
	void generate();//�հ������������һ����
	void output_board();//�������

private:
	Board board;
	bool move_valid;
	time_t begin_time, end_time;
};

#endif // !GUARD_Game2048_h
