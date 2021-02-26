#include "Header.h"
#include <string.h>

void cal(int *arr, int p) {
	int i,cur,n;

	memset(arr, 0, sizeof(int)*10);		//arr를 0으로 초기화

	for (i = 0; i < p; i++) {			//1페이지부터 p페이지까지 검사
		cur = i + 1;					//cur : 현재 페이지
		while (cur > 0) {
			n = cur % 10;				//n : cur의 1의 자릿수
			arr[n]++;					//해당하는 자릿수에 +1
			cur = cur / 10;				//다음 숫자를 조사하기 위해 cur에 10을 나눈값을 다시 cur에 저장 
		}
	}

}