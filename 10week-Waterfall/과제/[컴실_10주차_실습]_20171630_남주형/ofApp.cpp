#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	ofSetLineWidth(5);

	water_flag = 0;
	draw_flag = 0;
	load_flag = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetLineWidth(5);


	if (draw_flag) {

		for (int i = 0; i < n_line; i++) {
			ofSetColor(ofColor::black);
			ofDrawLine(LS[i].x1, LS[i].y1, LS[i].x2, LS[i].y2);
		}
		for (int i = 0; i < n_dot; i++) {
			if (DS[i].button)
				ofSetColor(ofColor::red);
			else
				ofSetColor(ofColor::black);

			ofDrawCircle(DS[i].x1, DS[i].y1, 10);
		}
		if (water_flag) {
			for (int i = 0; i < n_dot; i++) {
				if (DS[i].button) {
					ofSetLineWidth(4);
					wl.draw();
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd' || key == 'D') {
		if (!load_flag) {
			cout << "file does not exists" << endl;
			return;
		}
		draw_flag = 1;
	}
	if (key == 'q' || key == 'Q') {
		cout << "Quit" << endl;
		_Exit(0);
	}
	if (key == 's') {
		if (draw_flag == 1 && load_flag == 1) {
			if (water_flag == 0) {
				cout << "water on" << endl;
				wl.computation(n_line, LS, DS[cur]);
				water_flag = 1;
			}
		}
	}
	if (key == 'e') {
		if (draw_flag == 1 && load_flag == 1) {
			if (water_flag == 1) {
				cout << "water off" << endl;
				water_flag = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l' || key == 'L') {

		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select txt for Waterfall");

		if (openFileResult.bSuccess) {
			processOpenFileSelection(openFileResult);
			load_flag = 1;
		}
	}
	if (key == OF_KEY_RIGHT) {
		if (water_flag == 0) {
			prev = cur;
			if (cur == n_dot - 1)
				cur = 0;
			else
				cur++;
			DS[cur].button = 1;
			DS[prev].button = 0;

			cout << "Selcted Dot Coordinate is (" << DS[cur].x1 << ", " << DS[cur].y1 << ")" << endl;
		}
	}
	if (key == OF_KEY_LEFT) {
		if (water_flag == 0) {
			prev = cur;
			if (cur == 0)
				cur = n_dot - 1;
			else
				cur--;
			DS[cur].button = 1;
			DS[prev].button = 0;

			cout << "Selcted Dot Coordinate is (" << DS[cur].x1 << ", " << DS[cur].y1 << ")" << endl;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	int flag = 0;
	int l = 0, d = 0;


	ofFile file(openFileResult.getName());

	if (!file.exists()) cout << "Target file does not exists." << endl;
	else cout << "We found the target file." << endl;

	ofBuffer buffer(file);

	if (buffer.size()) {

		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
			string line = *it;

			vector<string> words = ofSplitString(line, " ");

			if (words.size() == 1) {
				if (flag == 0) {
					n_line = atoi(words[0].c_str());
					cout << "The number of line is: " << n_line << endl;
					LS = new LineSource[n_line];
				}
				else {
					n_dot = atoi(words[0].c_str());
					cout << "The number of dot is: " << n_dot << endl;
					DS = new DotSource[n_dot];
				}
			}
			else if (words.size() >= 2) {
				int x1, y1, x2, y2;
				if (flag == 0) {
					x1 = atoi(words[0].c_str());
					y1 = atoi(words[1].c_str());
					x2 = atoi(words[2].c_str());
					y2 = atoi(words[3].c_str());

					if (x1 < 0 || x1 > ofGetWidth()) {
						return;
					}
					if (x2 < 0 || x2 > ofGetWidth()) {
						return;
					}
					if (y1 < 0 || y1 > ofGetHeight()) {
						return;
					}
					if (y2 < 0 || y2 > ofGetHeight()) {
						return;
					}

					LS[l].x1 = x1;
					LS[l].y1 = y1;
					LS[l].x2 = x2;
					LS[l].y2 = y2;
					LS[l].slope = (float)(y2 - y1) / (float)(x2 - x1);
					l++;
					if (l == n_line)
						flag = 1;
				}
				else {
					x1 = atoi(words[0].c_str());
					y1 = atoi(words[1].c_str());
					if (x1 < 0 || x1 > ofGetScreenWidth()) {
						return;
					}
					if (y1 < 0 || y1 >ofGetScreenHeight()) {
						return;
					}
					DS[d].x1 = x1;
					DS[d].y1 = y1;
					d++;
				}
			}
		}
	}
	quickSort(0, n_dot - 1);
	DS[0].button = 1;
	wl = waterObj(n_line);
}

void ofApp::Dotswap(DotSource *a, DotSource *b) {
	DotSource temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void ofApp::quickSort(int start, int end) {
	if (start >= end)return;
	int key = start, i = start + 1, j = end;
	while (i <= j) {//i가 j보다 커진순간은 엇갈린순간
		while (i <= end && DS[i].x1 <= DS[key].x1)i++;
		while (j > start && DS[j].x1 >= DS[key].x1)j--;
		if (i > j) Dotswap(&DS[key], &DS[j]);
		else Dotswap(&DS[i], &DS[j]);
	}
	quickSort(start, j - 1);//기준점의 왼쪽을 다시 정렬
	quickSort(j + 1, end);//기준점의 오른쪽을 다시정렬
}
