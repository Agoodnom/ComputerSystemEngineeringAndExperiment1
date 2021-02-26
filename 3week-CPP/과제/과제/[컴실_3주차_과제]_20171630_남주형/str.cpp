#include "str.h"
#include <string.h>
#include <iostream>

using namespace std;

Str::Str(int leng) {
	str = new char(leng + 1);
	len = leng;
}

Str::Str(const char *neyong) {
	len = strlen(neyong);
	str = new char[len + 1];
	strcpy(str, neyong);
}

Str::~Str() {
	delete[]str;
}

int Str::length(void) {
	return len;
}

char* Str::contents(void) {
	return str;
}

int Str::compare(class Str& a) {
	return strcmp(str, a.contents());
}

int Str::compare(const char *a) {
	return strcmp(str, a);
}

void Str::operator=(const char *a) {
		delete[] str;
		len = strlen(a);
		str = new char[len + 1];
		strcpy(str, a);

}

void Str::operator=(class Str& a) {
		delete[]str;
		len = a.length();
		str = new char[len + 1];
		strcpy(str, a.contents());
}