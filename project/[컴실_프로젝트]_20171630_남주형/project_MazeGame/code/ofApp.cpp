/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
#define CELL_SIZE 6
#define WALL_WIDTH 1
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(0, 0, 0, 0);	//���ȭ���� ���������� ����
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	//flag �ʱ�ȭ
	ball_1p_flag = false;
	ball_2p_flag = false;
	game1_end = false;
	game2_end = false;
	game2_end_1 = false;
	game2_end_2 = false;
	isOpen = 0;
	//�̹��� �ε�
	clear.load("clear.png");
	player1.load("player1win.png");
	player2.load("player2win.png");

	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	// Add popup items to the File menu

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	// View popup menu
	hPopup = menu->AddPopupMenu(hMenu, "View");

	menu->AddPopupItem(hPopup, "Play 1p", false, false); // Checked
	menu->AddPopupItem(hPopup, "Play 2p", false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	// Help popup menu
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();



} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		if (isOpen) {
			freeMemory();	//�޸� �Ҵ�����
			//flag �ʱ�ȭ
			ball_1p_flag = false;
			ball_2p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;
		}

		readFile();	//���� �б� �� ���� ����
	}
	if (title == "Exit") {
		cout << "Quit" << endl;
		freeMemory();	//�޸� �Ҵ�����
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Play 1p") {	//1�ο� ���� ����
		if (isOpen)
		{
			ball_1p_flag = true;	//flag ����
			//flag �ʱ�ȭ
			ball_2p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;

			ball_set();	//ball��ġ ����
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Play 2p") {
		if (isOpen)
		{
			ball_2p_flag = true;	//flag ����
			//flag �ʱ�ȭ
			ball_1p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;

			ball_set();	//ball��ġ ����
		}
		else
			cout << "you must open file first" << endl;
	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

	//windowâ�� ũ�⿡ �°� �̷θ� �׷��ֱ� ���� size�� �������־���.
	size = (double)ofGetHeight() / (double)(CELL_SIZE * ((rows) / 2) + WALL_WIDTH * (rows - (rows) / 2));
	if (size > (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2)))
		size = (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2));
}


//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0, 0, 0, 0);	//���ȭ���� ���������� ����

	maze_draw();	//�̷θ� �׷��ش�.

	if (ball_1p_flag || ball_2p_flag) {	//flag�� �������� Ȯ��
		ball_draw();	//ball�� �׷��ش�.
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
	}

} // end doFullScreen


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {	//ESC key ������ ��üȭ�� ����, ��üȭ���� �ƴϸ� ����
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			cout << "Quit" << endl;
			freeMemory();	//�޸� �Ҵ�����
			ofExit();	//����
		}
	}

	if (key == OF_KEY_LEFT) {	//���� ����Ű�� ������
		if (ball_1p_flag || ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_1p.second;	//ball_1p�� x ��ǥ�� bx�� ����
			int by = ball_1p.first;		//ball_1p�� y ��ǥ�� by�� ����
			if (field[by][bx - 1] == ' ') {	//���ʿ� ���� �ִ��� ������ Ȯ��
				ball_1p.second -= 2;	//���� ���ٸ� ball_1p�� x���� -2�� �������־� ball_1p�� ���� CELL�� �̵���Ų��.
			}
		}
	}
	if (key == OF_KEY_RIGHT) {	//������ ����Ű�� ������
		if (ball_1p_flag || ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_1p.second;	//ball_1p�� x ��ǥ�� bx�� ����
			int by = ball_1p.first;		//ball_1p�� y ��ǥ�� by�� ����
			if (field[by][bx + 1] == ' ') {	//�����ʿ� ���� �ִ��� ������ Ȯ��
				ball_1p.second += 2;	//���� ���ٸ� ball_1p�� x���� +2�� �������־� ball_1p�� ������ CELL�� �̵���Ų��.
			}
		}
	}
	if (key == OF_KEY_UP) {	//���� ����Ű�� ������
		if (ball_1p_flag || ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_1p.second;	//ball_1p�� x ��ǥ�� bx�� ����
			int by = ball_1p.first;		//ball_1p�� y ��ǥ�� by�� ����
			if (field[by - 1][bx] == ' ') {	//���ʿ� ���� �ִ��� ������ Ȯ��
				ball_1p.first -= 2;		//���� ���ٸ� ball_1p�� y���� -2�� �������־� ball_1p�� ���� CELL�� �̵���Ų��.
			}
		}
	}
	if (key == OF_KEY_DOWN) {	//�Ʒ��� ����Ű�� ������
		if (ball_1p_flag || ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_1p.second;	//ball_1p�� x ��ǥ�� bx�� ����
			int by = ball_1p.first;		//ball_1p�� y ��ǥ�� by�� ����
			if (field[by + 1][bx] == ' ') {	//�Ʒ��ʿ� ���� �ִ��� ������ Ȯ��
				ball_1p.first += 2;		//���� ���ٸ� ball_1p�� y���� +2�� �������־� ball_1p�� �Ʒ��� CELL�� �̵���Ų��.
			}
		}
	}
	if (key == 'a' || key == 'A') {	//'a','A' ����Ű�� ������
		if (ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_2p.second;	//ball_2p�� x ��ǥ�� bx�� ����
			int by = ball_2p.first;		//ball_2p�� y ��ǥ�� by�� ����
			if (field[by][bx - 1] == ' ') {	//���ʿ� ���� �ִ��� ������ Ȯ��
				ball_2p.second -= 2;	//���� ���ٸ� ball_2p�� x���� -2�� �������־� ball_2p�� ���� CELL�� �̵���Ų��.
			}
		}
	}
	if (key == 'd' || key == 'D') {	//'d','D' ����Ű�� ������
		if (ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_2p.second;	//ball_2p�� x ��ǥ�� bx�� ����
			int by = ball_2p.first;		//ball_2p�� y ��ǥ�� by�� ����
			if (field[by][bx + 1] == ' ') {	//�����ʿ� ���� �ִ��� ������ Ȯ��
				ball_2p.second += 2;	//���� ���ٸ� ball_2p�� x���� +2�� �������־� ball_2p�� ������ CELL�� �̵���Ų��.
			}
		}
	}
	if (key == 'w' || key == 'W') {	//'w','W' ����Ű�� ������
		if (ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_2p.second;	//ball_2p�� x ��ǥ�� bx�� ����
			int by = ball_2p.first;		//ball_2p�� y ��ǥ�� by�� ����
			if (field[by - 1][bx] == ' ') {	//���ʿ� ���� �ִ��� ������ Ȯ��
				ball_2p.first -= 2;		//���� ���ٸ� ball_2p�� y���� -2�� �������־� ball_2p�� ���� CELL�� �̵���Ų��.
			}
		}
	}
	if (key == 's' || key == 'S') {	//'s','S' ����Ű�� ������
		if (ball_2p_flag) {	//ball flag�� �������� ��
			if (game1_end || game2_end)return;	//end flag�� �����ƴٸ� �״�� retrun
			int bx = ball_2p.second;	//ball_2p�� x ��ǥ�� bx�� ����
			int by = ball_2p.first;		//ball_2p�� y ��ǥ�� by�� ����
			if (field[by + 1][bx] == ' ') {	//�Ʒ��ʿ� ���� �ִ��� ������ Ȯ��
				ball_2p.first += 2;		//���� ���ٸ� ball_2p�� y���� +2�� �������־� ball_2p�� �Ʒ��� CELL�� �̵���Ų��.
			}
		}
	}
} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

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
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()	//������ �а� ������ �����ϴ� �Լ�
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		cout << "Open" << endl;
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {	//maz�����϶��� ������ �д´�.

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);
			if (buffer.size()) {
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {	//������ ���پ� �д´�.
					string line = *it;	//line�� ������ �����Ѵ�.
					vector<char> words(line.begin(), line.end());	//line�� ó������ ���������� words�� char������ �����Ѵ�.
					field.emplace_back(words);	//field�� words�� ������ �����Ѵ�. �̷��� ������ ���������� field�� �����Ѵ�.
				}
			}
			rows = field.size();	//rows�� ����
			cols = field[0].size();	//cols�� ����

			//dfs�� �ʿ��� ������ �Ҵ��Ѵ�.
			visited.resize(rows);
			shortest.resize(rows);
			for (int i = 0; i < rows; i++)
				visited[i].resize(cols);
			for (int i = 0; i < rows; i++)
				shortest[i].resize(cols);
		}
		else {
			cout << "  Needs a '.maz' extension" << endl;
			return false;
		}
	}
}
void ofApp::freeMemory() {	//�޸� �Ҵ����� �Լ�
	cols = 0;	//cols�� 0���� �ʱ�ȭ
	rows = 0;	//rows�� 0���� �ʱ�ȭ
	//�Ҵ��ߴ� �������� �Ҵ����� �����ش�.
	field.clear();
	visited.clear();
	shortest.clear();
}

bool ofApp::DFS(pair<int, int> start, pair<int, int> destination)//DFSŽ���� �ϴ� �Լ�
{
	int nx, ny;
	int x, y;
	//visited, shortest�� 0���� �ʱ�ȭ
	for (int i = 0; i < rows; i++)
		fill(visited[i].begin(), visited[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(shortest[i].begin(), shortest[i].end(), false);

	vector< vector< pair<int, int>>> prev_cell;	//���� ��ǥ�� ����ϱ� ���� ����
	//rows�� cols�� ũ�⸸ŭ �Ҵ�
	prev_cell.resize(rows);
	for (int i = 0; i < rows; i++)
		prev_cell[i].resize(cols);

	stack< pair<int, int>> s;	//���� s ����
	s.emplace(make_pair(start.first, start.second)); //���� s�� ������ǥ�� �ִ´�.
	visited[start.first][start.second] = true;	//���� ��ǥ�� �湮 ǥ�� ���ش�.

	bool flag = false;	//�������� �����ߴ��� Ȯ���ϱ� ���� flag�� �ʱ�ȭ ���ش�.

	while (s.size() && !flag) {	//���� s�� ����ų� flag�� 1�̸� ����
		y = s.top().first;	//y�� ���� s�� top�� �ִ� y��ǥ�� ����
		x = s.top().second;	//x�� ���� s�� top�� �ִ� x��ǥ�� ����
		s.pop();	//���� s�� pop

		for (int d = 0; d < 4; d++) {
			//�����¿쿡 �ִ� cell�� �˻� 
			if (destination.first - start.first > 0 && destination.second - start.second > 0) {	//������� ���������� �������� �������(1p�� ���)
				nx = x + dx[d];
				ny = y + dy[d];
			}
			else {	//������� ���������� �����ʾƷ��� �������(2p�� ���)
				nx = x + dx[3 - d];
				ny = y + dy[3 - d];
			}

			if (visited[ny][nx])	//�湮�� �̷��� ������ �Ѿ��.
				continue;
			if (field[ny][nx] != ' ')	//������ ���������� �Ѿ��.
				continue;

			visited[ny][nx] = true;	//���� ���ǿ� �ش���� �ʴ´ٸ� �湮ǥ��
			s.emplace(make_pair(ny, nx));	//���� s�� �־��ش�.
			prev_cell[ny][nx] = make_pair(y, x);	//���� ��ǥ ���� ��ġ�� ����

			if (nx == destination.second && ny == destination.first) {
				flag = true;	//���� ��ǥ�� �������� �����ߴٸ� flag�� �����ϰ� break
				break;
			}
		}
	}

	if (flag) {//flag�� ���õǾ��ٸ�
		int x = destination.second;
		int y = destination.first;
		shortest[y][x] = true; //�������� ǥ��

		while (x != start.second || y != start.first) {	//x,y��ǥ�� �������� �����ϸ� ����
			//���� ��ǥ�� px,py�� ����
			int px = prev_cell[y][x].second;
			int py = prev_cell[y][x].first;
			shortest[py][px] = true;	//�ִܰŸ��� ��ǥ�� ������Ʈ���ش�.
			x = px, y = py;	//x,y�� px,py�� �ٲپ��־� ��� ���� ��ġ�� ã�ư��� ���ش�.
		}
	}

	return 0;
}

void ofApp::route_draw(pair<int, int> start, pair<int, int> destination) {	//�ִܰŸ��� �׷��ִ� �Լ�
	for (int i = 0; i < rows; i++) {
		double y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		y *= size;	//y���� ������ְ� size�� �����ش�.
		for (int j = 0; j < cols; j++) {
			double x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
				(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
			x *= size;	//x���� ������ְ� size�� �����ش�.
			if (shortest[i][j]) { //�ش� ��ǥ�� �ִܰŸ� ��ǥ�� �ش��ϸ� �׷��ش�.
				ofSetColor(200);	//�ִܰŸ��� ȸ������ ǥ��
				if ((i == start.first && j == start.second) || (i == destination.first && j == destination.second)) {
					ofSetColor(ofColor::green);	//������� �������� �ʷϻ����� ǥ��
				}
				if ((i % 2) && (j % 2)) {	//i,j�� Ȧ�� �̸�
					//���簢���� �׷��ش�.
					ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				}
				else if (i % 2) {	//i�� Ȧ�� �̸�
					//'-'����� ���簢���� �׷��ش�.
					ofDrawRectangle(x - CELL_SIZE * size + WALL_WIDTH * size, y, CELL_SIZE*size, WALL_WIDTH*size);
				}
				else {	//�� �� '|'����� ���簢���� �׷��ش�.
					ofDrawRectangle(x, y - CELL_SIZE * size + WALL_WIDTH * size, WALL_WIDTH*size, CELL_SIZE*size);
				}
			}
		}
	}
}

void ofApp::maze_draw() {	//�̷θ� �׷��ִ� �Լ�
	int i, j;
	for (int i = 0; i < rows; i++) {
		int y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2);
		y *= size;	//y���� ������ְ� size�� �����ش�
		for (int j = 0; j < cols; j++) {
			int x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2);
			x *= size;	//x���� ������ְ� size�� �����ش�.
			ofSetColor(ofColor::white);	//�̷θ� ������� �׷��ش�.

			switch (field[i][j]) {
			case '+':	//'+'�̸� ���簢���� �׷��ش�.
				ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				break;
			case '-':	//'-'�̸� �¿�� ������ ����� ���簢���� �׷��ش�.
				ofDrawRectangle(x, y, CELL_SIZE*size, WALL_WIDTH*size);
				break;
			case '|':	//'|'�̸� ���Ϸ� ������ ����� ���簢���� �׷��ش�.
				ofDrawRectangle(x, y, WALL_WIDTH*size, CELL_SIZE*size);
				break;
			}
		}
	}
}

void ofApp::ball_set() {	//ball�� ��ġ�� �����ϴ� �Լ�
	if (ball_1p_flag) {	//1�ο� ������ ����������
		ball_1p = make_pair(1, 1);	//���������� (1(=y),1(=x))�� �������ش�.
		start1 = make_pair(1, 1);	//���������� ������ش�.
		dest = make_pair(rows - 2, cols - 2);	//�������� (rows-2(=y),cols-2(=x))�� �������ش�.
	}
	if (ball_2p_flag) {	//2�ο� ������ ����������
		ball_1p = make_pair(1, 1);	//ball_1p�� ���������� (1(=y),1(=x))�� �������ش�.
		start1 = make_pair(1, 1);	//ball_1p�� ���������� ������ش�.
		ball_2p = make_pair(rows - 2, cols - 2);	//ball_2p�� ���������� (rows-2(=y),cols-2(=x))�� �������ش�.
		start2 = make_pair(rows - 2, cols - 2);		//ball_2p�� ���������� ������ش�.

		//2�ο���ӿ��� �������� ball_1p�� ball_2p�� �߰��������� �������ش�.
		int y = (rows - 1) / 2;
		int x = (cols - 1) / 2;
		if (x % 2 == 0)
			x--;
		if (y % 2 == 0)
			y++;
		dest = make_pair(y, x);	//�������� �������ش�.
	}
}

void ofApp::ball_draw() {//ball�� ��ġ�� ������Ʈ �ϸ� �׷��ְ� ������ �������� ����޼����� ����ִ� �Լ��̴�.

	if (ball_1p_flag) {	//1�ο� ������ ��
		//i,j�� �������� ��ǥ�� �����Ѵ�.
		int i = dest.first;
		int j = dest.second;
		//�̷ο� ǥ�õ� �������� ���� ��ǥ�� ������ش�.
		double dy = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dy *= size;
		double dx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dx *= size;
		//�������� ������ ��������� �������ְ� ������ ǥ�����ش�.
		ofSetColor(ofColor::yellow);
		ofDrawCircle(dx, dy, CELL_SIZE*size / 3);

		//i,j�� ball_1p�� ��ǥ�� �����Ѵ�.
		i = ball_1p.first;
		j = ball_1p.second;
		//�̷ο� ǥ�õ� ball_1p�� ���� ��ǥ�� ������ش�.
		double by = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		by *= size;
		double bx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		bx *= size;
		//ball_1p�� ������ ���������� �������ְ� ������ ǥ�����ش�.
		ofSetColor(ofColor::red);
		ofDrawCircle(bx, by, CELL_SIZE*size / 3);


		if (bx == dx && by == dy) {	//ball_1p�� �������� �����ߴٸ�
			game1_end = true;	//end flag�� true�� ����
			game_end();
		}

	}
	if (ball_2p_flag) {	//2�ο� ������ ��
		//i,j�� �������� ��ǥ�� �����Ѵ�.
		int i = dest.first;
		int j = dest.second;
		//�̷ο� ǥ�õ� �������� ���� ��ǥ�� ������ش�.
		double dy = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dy *= size;
		double dx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dx *= size;
		//�������� ������ ��������� �������ְ� ������ ǥ�����ش�.
		ofSetColor(ofColor::yellow);
		ofDrawCircle(dx, dy, CELL_SIZE*size / 3);

		//i,j�� ball_1p�� ��ǥ�� �����Ѵ�.
		i = ball_1p.first;
		j = ball_1p.second;
		//�̷ο� ǥ�õ� ball_1p�� ���� ��ǥ�� ������ش�.
		double b1y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b1y *= size;
		double b1x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b1x *= size;
		//ball_1p�� ������ ���������� �������ְ� ������ ǥ�����ش�.
		ofSetColor(ofColor::red);
		ofDrawCircle(b1x, b1y, CELL_SIZE*size / 3);

		//i,j�� ball_2p�� ��ǥ�� �����Ѵ�.
		i = ball_2p.first;
		j = ball_2p.second;
		//�̷ο� ǥ�õ� ball_2p�� ���� ��ǥ�� ������ش�.
		double b2y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b2y *= size;
		double b2x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b2x *= size;
		//ball_2p�� ������ ���������� �������ְ� ������ ǥ�����ش�.
		ofSetColor(ofColor::blue);
		ofDrawCircle(b2x, b2y, CELL_SIZE*size / 3);


		if (b1x == dx && b1y == dy) {	//ball_1p�� �������� �����ߴٸ�
			//end flag�� true�� ����
			game2_end = true;
			game2_end_1 = true;
			game_end();
		}
		if (b2x == dx && b2y == dy) {	//ball_2p�� �������� �����ߴٸ�
			//end flag�� true�� ����
			game2_end = true;
			game2_end_2 = true;
			game_end();
		}
	}
}
void ofApp::game_end() {

	if (game1_end) {//1�ο���� clear ���� ��
		DFS(start1, dest);	//DFS�� Ȱ���Ͽ� ball_1p�� ������������ ���������� �ִܰ�θ� �����ش�.
		route_draw(start1, dest);	//�ִܰ�θ� �׷��ش�.
		ofSetColor(ofColor::white);	//�Ͼ������ ����
		clear.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);//clear�޼����� ����ش�.
	}
	else if (game2_end) {//2�ο���� clear ���� ��
		if (game2_end_1) {//1p�� �̰��� ��
			DFS(start1, dest);	//DFS�� Ȱ���Ͽ� ball_1p�� ������������ ���������� �ִܰ�θ� �����ش�.
			route_draw(start1, dest);	//�ִܰ�θ� �׷��ش�.
			DFS(start2, dest);	//DFS�� Ȱ���Ͽ� ball_2p�� ������������ ���������� �ִܰ�θ� �����ش�.
			route_draw(start2, dest);	//�ִܰ�θ� �׷��ش�.
			ofSetColor(ofColor::white);	//�Ͼ������ ����
			player1.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);	//'player 1 win!'�޼����� ����ش�.


		}
		else if (game2_end_2) {//2p�� �̰��� ��
			DFS(start1, dest);	//DFS�� Ȱ���Ͽ� ball_1p�� ������������ ���������� �ִܰ�θ� �����ش�.
			route_draw(start1, dest);	//�ִܰ�θ� �׷��ش�.
			DFS(start2, dest);	//DFS�� Ȱ���Ͽ� ball_2p�� ������������ ���������� �ִܰ�θ� �����ش�.
			route_draw(start2, dest);	//�ִܰ�θ� �׷��ش�.
			ofSetColor(ofColor::white);		//�Ͼ������ ����
			player2.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);	//'player 2 win!'�޼����� ����ش�.
		}
	}
}