#include "waterObj.h"
#include "ofApp.h"

waterObj::waterObj() {

}

waterObj::waterObj(int line_num) {

	num_of_path = line_num * 2 + 2;
	path = new coordinate[num_of_path];
	for (int i = 0; i < num_of_path; i++)
		path[i].x = path[i].y = 0;

}

waterObj::~waterObj() {
}

void waterObj::draw() {
	ofSetColor(ofColor::blue);


	for (int i = 0; i < path_cnt-1; i++) {
		ofDrawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
	}


}

void waterObj::computation(int line_num, struct line *l, struct dot d) {
	path[0].x = d.x1;
	path[0].y = d.y1;
	int i, j = 0;
	int temp;
	int find_flag;
	int min;

	while (1) {
		min = 10000;
		find_flag = 0;
		for (i = 0; i < line_num; i++) {
			if (path[j].x >= l[i].x1&&path[j].x <= l[i].x2) {
				temp = l[i].slope * (path[j].x - l[i].x1) + l[i].y1;
				if (path[j].y < temp) {
					if (temp < min) {
						path[j + 1].x = path[j].x;
						path[j + 1].y = temp;
						if (l[i].slope > 0) {
							path[j + 2].x = l[i].x2;
							path[j + 2].y = l[i].y2;
						}
						else {
							path[j + 2].x = l[i].x1;
							path[j + 2].y = l[i].y1;
						}
						min = temp;
						find_flag = 1;
					}
				}
			}
		}
		if (!find_flag) {
			path[j + 1].x = path[j].x;
			path[j + 1].y = ofGetScreenHeight();
			path_cnt = j + 2;
			break;
		}
		j += 2;
	}

}