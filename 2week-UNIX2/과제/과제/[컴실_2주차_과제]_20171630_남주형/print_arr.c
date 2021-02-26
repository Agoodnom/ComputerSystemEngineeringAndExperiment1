#include "Header.h"

void print_arr(int *arr) {
	int i = 0;

	for (i = 0; i < 10; i++) {		//i를 0~9까지 1씩 증가
		printf("%d ", arr[i]);		//arr[i]출력(arr[i]는 각각의 숫자가 나오는 개수)
	}
	printf("\n");					//개행
}