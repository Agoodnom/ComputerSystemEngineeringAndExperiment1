#include "Header.h"

int main() {
	int i, n, p;									//n : testcase 수, p : 페이지 수
	int **arr;

	scanf("%d", &n);								//testcase 수 입력

	arr = (int**)malloc(sizeof(int*)*n);			
	for (i = 0; i < n; i++) {
		arr[i] = (int*)malloc(sizeof(int) * 10);
	}												//2차원 배열 생성(arr[n][100])

	for (i = 0; i < n; i++) {						//n번 시행 ,i는 1씩증가
		scanf("%d", &p);							//페이지 수 입력
		cal(arr[i], p);								//cal함수를 통해 계산
	}

	for (i = 0; i < n; i++) {						//n번 시행 ,i는 1씩증가
		print_arr(arr[i]);							//결과 출력
		free(arr[i]);								//할당해제
	}
	free(arr);										//할당해제

	return 0;										//종료
}