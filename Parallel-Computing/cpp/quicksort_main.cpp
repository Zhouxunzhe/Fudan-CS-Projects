//#include <stdio.h>
//#include <omp.h>
//#include <stdlib.h>
//#include <time.h>
//#include "quickSort.h"
//
//void test(int n) {
//	omp_set_num_threads(8);
//	double omp_time_begin, omp_time_end;
//	double seq_time_begin, seq_time_end;
//	double omp_time, seq_time;
//	int* data1, * data2;
//	int num = 8;
//	data1 = (int*)malloc(sizeof(int) * n);
//	data2 = (int*)malloc(sizeof(int) * n);
//	rands(data1, n);
//	rands(data2, n);
//
//	//���п�������
//	omp_time_begin = omp_get_wtime();
//	quickSort_parallel(data1, n, num);
//	omp_time_end = omp_get_wtime();
//	omp_time = omp_time_end - omp_time_begin;
//
//	//���п�������
//	seq_time_begin = omp_get_wtime();
//	quickSort(data2, 0, n - 1);
//	seq_time_end = omp_get_wtime();
//	seq_time = seq_time_end - seq_time_begin;
//
//	//�������ʱ��
//	printf("-------------------\n");
//	printf("���������� : %d\n", n);
//	printf("���д���ʱ�� : %lf s\n", omp_time);
//	printf("���д���ʱ�� : %lf s\n", seq_time);
//	printf("���ٱ� : %lf\n", seq_time / omp_time);
//	printf("-------------------\n");
//	printf("\n");
//
//	//�ͷ��ڴ�
//	free(data1);
//	free(data2);
//}
//
//int main()
//{
//	test(1000);
//	test(5000);
//	test(10000);
//	test(100000);
//	test(1000000);
//	test(10000000);
//	test(100000000);
//	return 0;
//}
