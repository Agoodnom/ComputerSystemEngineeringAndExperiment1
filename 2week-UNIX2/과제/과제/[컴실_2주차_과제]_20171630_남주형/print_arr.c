#include "Header.h"

void print_arr(int *arr) {
	int i = 0;

	for (i = 0; i < 10; i++) {		//i�� 0~9���� 1�� ����
		printf("%d ", arr[i]);		//arr[i]���(arr[i]�� ������ ���ڰ� ������ ����)
	}
	printf("\n");					//����
}