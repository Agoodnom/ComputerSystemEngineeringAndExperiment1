#include<iostream>
using namespace std;

#ifndef __ARRAY__
#define __ARRAY__

template <class T>
class Array {
protected:
	T *data;
	int len;
public:
	Array(int size) {
		if (size >= 0) {
			data = new T[size];
			len = size;
		}
		else {
			cout << "Array bound error!" << endl;
		}
	}
	~Array() {
		delete[] data;
	}
	int length() {
		return len;
	}
	virtual T& operator[] (int i) {
		static T tmp;

		if (i >= 0 && i < len)
			return data[i];

		cout << "Array bound error!" << endl;
		return tmp;
	}
	virtual T operator[] (int i) const {
		if (i >= 0 && i < len)
			return data[i];

		cout << "Array bound error!" << endl;
		return 0;
	}
	void print() {
		int i;

		cout << " [";
		for (i = 0; i < len - 1; i++) {
			cout << data[i] << " ";
		}
		cout << data[i] << "]" << endl;
	}
};
#endif