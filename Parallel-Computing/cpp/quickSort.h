#include "omp.h"
#include <stdlib.h>
//随机创建数组
void rands(int* data, int sum);
//交换函数
void swap(int* a, int* b);
//求2的n次幂
int exp2(int wht_num);
//求log2(n)
int log2(int wht_num);
//合并两个有序的数组
void mergeList(int* c, int* a, int sta1, int end1, int* b, int sta2, int end2);
//串行快速排序
int partition(int* a, int sta, int end);
void quickSort(int* a, int sta, int end);
//openMP(8)并行快速排序
void quickSort_parallel(int* array, int lenArray, int numThreads);
void quickSort_parallel_internal(int* array, int left, int right, int cutoff);

void rands(int* data, int sum) {
	int i;
	for (i = 0; i < sum; i++)
	{
		data[i] = rand() % 1000000000;
	}
}

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}
int exp2(int wht_num) {
	int wht_i;
	wht_i = 1;
	while (wht_num > 0)
	{
		wht_num--;
		wht_i = wht_i * 2;
	}
	return wht_i;
}
int log2(int wht_num) {
	int wht_i, wht_j;
	wht_i = 1;
	wht_j = 2;
	while (wht_j < wht_num)
	{
		wht_j = wht_j * 2;
		wht_i++;
	}
	if (wht_j > wht_num)
		wht_i--;
	return wht_i;
}
int partition(int* a, int sta, int end) {
	int i = sta, j = end + 1;
	int x = a[sta];
	while (1)
	{
		while (a[++i] < x && i < end);
		while (a[--j] > x);
		if (i >= j)
			break;
		swap(&a[i], &a[j]);
	}
	a[sta] = a[j];
	a[j] = x;
	return j;
}
//并行openMP排序
void quickSort_parallel(int* a, int lenArray, int numThreads) {
	int cutoff = 100;
#pragma omp parallel num_threads(numThreads) //指定线程数的数量
	{
#pragma omp single //串行执行
		{
			quickSort_parallel_internal(a, 0, lenArray - 1, cutoff);
		}
	}
}
void quickSort_parallel_internal(int* a, int left, int right, int cutoff) {
	int i = left, j = right;
	int tmp;
	int mid = a[(left + right) / 2];
	//进行数组分割，分成两部分（符合左小右大）
	while (i <= j)
	{
		while (a[i] < mid)
			i++;
		while (a[j] > mid)
			j--;
		if (i <= j) {
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
			i++;
			j--;
		}
	}
	//int j = partition(a,left,right);
	if (((right - left) < cutoff)) {
		if (left < j) {
			quickSort_parallel_internal(a, left, j, cutoff);
		}
		if (i < right) {
			quickSort_parallel_internal(a, i, right, cutoff);
		}
	}
	else {
#pragma omp task
		//对两部分再进行并行的线程排序
		{
			quickSort_parallel_internal(a, left, j, cutoff);
		}
#pragma omp task
		{
			quickSort_parallel_internal(a, i, right, cutoff);
		}
	}
}
//合并两个已排序的数组
void mergeList(int* c, int* a, int sta1, int end1, int* b, int sta2, int end2) {
	int a_index = sta1; // 遍历数组a的下标
	int b_index = sta2; // 遍历数组b的下标
	int i = 0;          // 记录当前存储位置
	while (a_index < end1 && b_index < end2) {
		if (a[a_index] <= b[b_index]) {
			c[i] = a[a_index];
			a_index++;
		}
		else {
			c[i] = b[b_index];
			b_index++;
		}
		i++;
	}
	while (a_index < end1) {
		c[i] = a[a_index];
		i++;
		a_index++;
	}
	while (b_index < end2) {
		c[i] = b[b_index];
		i++;
		b_index++;
	}
}
//串行快速排序
void quickSort(int* a, int sta, int end) {
	if (sta < end) {
		//printf("3\n");
		int mid = partition(a, sta, end);
		quickSort(a, sta, mid - 1);
		quickSort(a, mid + 1, end);
	}
}
