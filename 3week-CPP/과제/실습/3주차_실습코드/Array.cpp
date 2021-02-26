#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;

Array::Array(int size) {
	
}

Array::~Array() {
	
}

// 배열의 크기 리턴
int Array::length() const
{
	
}

// 배열에 원소를 대입하는 []연산자의 오버로딩이다
// 배열의 인덱스가 범위 내에 있으면 값 리턴, 그렇지 않으면 에러메세지 출력하고 tmp리턴
int& Array::operator[](int i) {
	static int tmp;
}

// 배열의 원소값 반환
// 배열의 인덱스가 범위 내에 잇으면 값을 리턴, 그렇지 않으면 에러메세지 출력하고 0을 리턴
int Array::operator[](int i) const {
	
}

//배열의 모든 내용을 출력해주는 함수
void Array::print() {
	
}
