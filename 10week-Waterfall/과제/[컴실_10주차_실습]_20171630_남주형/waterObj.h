#pragma once
#include "ofMain.h"

typedef struct{
	int x, y;
}coordinate;

class waterObj {
public:
	waterObj();
	waterObj(int line_num);
	~waterObj();

	void draw();
	void computation(int line_num, struct line *l, struct dot d);
	int path_cnt = 0;
	int num_of_path;

	coordinate *path;
	
};

