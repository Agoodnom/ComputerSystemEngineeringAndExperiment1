#include "Header.h"
#include <string.h>

void cal(int *arr, int p) {
	int i,cur,n;

	memset(arr, 0, sizeof(int)*10);		//arr�� 0���� �ʱ�ȭ

	for (i = 0; i < p; i++) {			//1���������� p���������� �˻�
		cur = i + 1;					//cur : ���� ������
		while (cur > 0) {
			n = cur % 10;				//n : cur�� 1�� �ڸ���
			arr[n]++;					//�ش��ϴ� �ڸ����� +1
			cur = cur / 10;				//���� ���ڸ� �����ϱ� ���� cur�� 10�� �������� �ٽ� cur�� ���� 
		}
	}

}