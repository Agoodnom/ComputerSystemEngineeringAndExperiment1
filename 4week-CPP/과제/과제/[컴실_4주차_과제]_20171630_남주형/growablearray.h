#include "array.h"
#include <iostream>
using namespace std;

template <class T>
class GrowableArray : public Array <T> {
public:
	GrowableArray(int size) : Array<T>(size) {}
	~GrowableArray() {}
	virtual T& operator[](int i) {
		int j = 0;
		if (i >= this->len) {
			T* tmp;
			tmp = this->data;
			this->data = new T[this->len * 2];

			for (; j < this->len; j++) {
				this->data[j] = tmp[j];
			}
			for (; j < this->len * 2; j++) {
				this->data[j] = 0;
			}
			delete[] tmp;
			this->len = this->len * 2;
			return this->data[i];
		}
		return Array<T>::operator[](i);
	}

	virtual T operator[](int i)const {
		return Array<T>::operator[](i);
	}

};