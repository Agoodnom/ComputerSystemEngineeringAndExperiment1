#include "Header.h"

int main() {
	int i, n, p;									//n : testcase ��, p : ������ ��
	int **arr;

	scanf("%d", &n);								//testcase �� �Է�

	arr = (int**)malloc(sizeof(int*)*n);			
	for (i = 0; i < n; i++) {
		arr[i] = (int*)malloc(sizeof(int) * 10);
	}												//2���� �迭 ����(arr[n][100])

	for (i = 0; i < n; i++) {						//n�� ���� ,i�� 1������
		scanf("%d", &p);							//������ �� �Է�
		cal(arr[i], p);								//cal�Լ��� ���� ���
	}

	for (i = 0; i < n; i++) {						//n�� ���� ,i�� 1������
		print_arr(arr[i]);							//��� ���
		free(arr[i]);								//�Ҵ�����
	}
	free(arr);										//�Ҵ�����

	return 0;										//����
}