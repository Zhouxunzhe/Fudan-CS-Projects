#ifndef GUARD_Board_h
#define GUARD_Board_h
#include<string>
#include<vector>
#include<time.h>
#include<stdlib.h>
#include<iostream>

class Board {
public:
	Board();//���캯����ʼ������
	int get_score() { return score; }//��ȡ�÷�
	void output_board();//�������
	void generate();//�հ������������һ����
	void move();//�������������ƶ�����
	void merge();//�ϲ����ֲ��Ʒ�
	bool is_full();//�ж������Ƿ�����
	bool is_merge();//�ж������Ƿ񻹿��Ժϲ�
	void trans();//������ת��
	void reverse();//��������ˮƽ��ת
	void updown();//�������ش�ֱ��ת

private:
	int score;//���ݳ�Ա������
	int board[4][4];//���ݳ�Ա������
};

#endif // !GUARD_Game2048_h