#pragma once
#include "ofMain.h"
#include "waterObj.h"


typedef struct line {
	int x1;
	int y1;
	int x2;
	int y2;
	float slope;
}LineSource;

typedef struct dot {
	int x1;
	int y1;
	bool button = 0;
}DotSource;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void quickSort(int start, int end);
	void Dotswap(DotSource *a, DotSource *b);

	int draw_flag;
	int load_flag;
	int water_flag;
	int n_line, n_dot;
	LineSource *LS;
	DotSource *DS;
	waterObj wl;
	int cur = 0, prev = 0;
};
