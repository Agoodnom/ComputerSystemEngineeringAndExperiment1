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
	ofBackground(0, 0, 0, 0);	//배경화면을 검은색으로 설정
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	//flag 초기화
	ball_1p_flag = false;
	ball_2p_flag = false;
	game1_end = false;
	game2_end = false;
	game2_end_1 = false;
	game2_end_2 = false;
	isOpen = 0;
	//이미지 로드
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
			freeMemory();	//메모리 할당해제
			//flag 초기화
			ball_1p_flag = false;
			ball_2p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;
		}

		readFile();	//파일 읽기 및 정보 저장
	}
	if (title == "Exit") {
		cout << "Quit" << endl;
		freeMemory();	//메모리 할당해제
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Play 1p") {	//1인용 게임 실행
		if (isOpen)
		{
			ball_1p_flag = true;	//flag 설정
			//flag 초기화
			ball_2p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;

			ball_set();	//ball위치 설정
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Play 2p") {
		if (isOpen)
		{
			ball_2p_flag = true;	//flag 설정
			//flag 초기화
			ball_1p_flag = false;
			game1_end = false;
			game2_end = false;
			game2_end_1 = false;
			game2_end_2 = false;

			ball_set();	//ball위치 설정
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

	//window창의 크기에 맞게 미로를 그려주기 위해 size를 설정해주었다.
	size = (double)ofGetHeight() / (double)(CELL_SIZE * ((rows) / 2) + WALL_WIDTH * (rows - (rows) / 2));
	if (size > (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2)))
		size = (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2));
}


//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0, 0, 0, 0);	//배경화면을 검은색으로 설정

	maze_draw();	//미로를 그려준다.

	if (ball_1p_flag || ball_2p_flag) {	//flag가 설정된지 확인
		ball_draw();	//ball을 그려준다.
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
	if (key == VK_ESCAPE) {	//ESC key 누르면 전체화면 해제, 전체화면이 아니면 종료
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			cout << "Quit" << endl;
			freeMemory();	//메모리 할당해제
			ofExit();	//종료
		}
	}

	if (key == OF_KEY_LEFT) {	//왼쪽 방향키를 누르면
		if (ball_1p_flag || ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_1p.second;	//ball_1p의 x 좌표를 bx에 저장
			int by = ball_1p.first;		//ball_1p의 y 좌표를 by에 저장
			if (field[by][bx - 1] == ' ') {	//왼쪽에 벽이 있는지 없는지 확인
				ball_1p.second -= 2;	//벽이 없다면 ball_1p의 x값을 -2로 설정해주어 ball_1p을 왼쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == OF_KEY_RIGHT) {	//오른쪽 방향키를 누르면
		if (ball_1p_flag || ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_1p.second;	//ball_1p의 x 좌표를 bx에 저장
			int by = ball_1p.first;		//ball_1p의 y 좌표를 by에 저장
			if (field[by][bx + 1] == ' ') {	//오른쪽에 벽이 있는지 없는지 확인
				ball_1p.second += 2;	//벽이 없다면 ball_1p의 x값을 +2로 설정해주어 ball_1p을 오른쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == OF_KEY_UP) {	//위쪽 방향키를 누르면
		if (ball_1p_flag || ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_1p.second;	//ball_1p의 x 좌표를 bx에 저장
			int by = ball_1p.first;		//ball_1p의 y 좌표를 by에 저장
			if (field[by - 1][bx] == ' ') {	//위쪽에 벽이 있는지 없는지 확인
				ball_1p.first -= 2;		//벽이 없다면 ball_1p의 y값을 -2로 설정해주어 ball_1p을 위쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == OF_KEY_DOWN) {	//아래쪽 방향키를 누르면
		if (ball_1p_flag || ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_1p.second;	//ball_1p의 x 좌표를 bx에 저장
			int by = ball_1p.first;		//ball_1p의 y 좌표를 by에 저장
			if (field[by + 1][bx] == ' ') {	//아래쪽에 벽이 있는지 없는지 확인
				ball_1p.first += 2;		//벽이 없다면 ball_1p의 y값을 +2로 설정해주어 ball_1p을 아래쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == 'a' || key == 'A') {	//'a','A' 방향키를 누르면
		if (ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_2p.second;	//ball_2p의 x 좌표를 bx에 저장
			int by = ball_2p.first;		//ball_2p의 y 좌표를 by에 저장
			if (field[by][bx - 1] == ' ') {	//왼쪽에 벽이 있는지 없는지 확인
				ball_2p.second -= 2;	//벽이 없다면 ball_2p의 x값을 -2로 설정해주어 ball_2p을 왼쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == 'd' || key == 'D') {	//'d','D' 방향키를 누르면
		if (ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_2p.second;	//ball_2p의 x 좌표를 bx에 저장
			int by = ball_2p.first;		//ball_2p의 y 좌표를 by에 저장
			if (field[by][bx + 1] == ' ') {	//오른쪽에 벽이 있는지 없는지 확인
				ball_2p.second += 2;	//벽이 없다면 ball_2p의 x값을 +2로 설정해주어 ball_2p을 오른쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == 'w' || key == 'W') {	//'w','W' 방향키를 누르면
		if (ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_2p.second;	//ball_2p의 x 좌표를 bx에 저장
			int by = ball_2p.first;		//ball_2p의 y 좌표를 by에 저장
			if (field[by - 1][bx] == ' ') {	//위쪽에 벽이 있는지 없는지 확인
				ball_2p.first -= 2;		//벽이 없다면 ball_2p의 y값을 -2로 설정해주어 ball_2p을 위쪽 CELL로 이동시킨다.
			}
		}
	}
	if (key == 's' || key == 'S') {	//'s','S' 방향키를 누르면
		if (ball_2p_flag) {	//ball flag가 설정됐을 때
			if (game1_end || game2_end)return;	//end flag가 설정됐다면 그대로 retrun
			int bx = ball_2p.second;	//ball_2p의 x 좌표를 bx에 저장
			int by = ball_2p.first;		//ball_2p의 y 좌표를 by에 저장
			if (field[by + 1][bx] == ' ') {	//아래쪽에 벽이 있는지 없는지 확인
				ball_2p.first += 2;		//벽이 없다면 ball_2p의 y값을 +2로 설정해주어 ball_2p을 아래쪽 CELL로 이동시킨다.
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
bool ofApp::readFile()	//파일을 읽고 정보를 저장하는 함수
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
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {	//maz파일일때만 파일을 읽는다.

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
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {	//파일을 한줄씩 읽는다.
					string line = *it;	//line에 한줄을 저장한다.
					vector<char> words(line.begin(), line.end());	//line을 처음부터 마지막까지 words에 char형으로 저장한다.
					field.emplace_back(words);	//field에 words의 내용을 저장한다. 미로의 정보를 최종적으로 field에 저장한다.
				}
			}
			rows = field.size();	//rows를 저장
			cols = field[0].size();	//cols를 저장

			//dfs에 필요한 변수를 할당한다.
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
void ofApp::freeMemory() {	//메모리 할당해제 함수
	cols = 0;	//cols를 0으로 초기화
	rows = 0;	//rows를 0으로 초기화
	//할당했던 변수들을 할당해제 시켜준다.
	field.clear();
	visited.clear();
	shortest.clear();
}

bool ofApp::DFS(pair<int, int> start, pair<int, int> destination)//DFS탐색을 하는 함수
{
	int nx, ny;
	int x, y;
	//visited, shortest를 0으로 초기화
	for (int i = 0; i < rows; i++)
		fill(visited[i].begin(), visited[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(shortest[i].begin(), shortest[i].end(), false);

	vector< vector< pair<int, int>>> prev_cell;	//이전 좌표를 기억하기 위한 변수
	//rows와 cols의 크기만큼 할당
	prev_cell.resize(rows);
	for (int i = 0; i < rows; i++)
		prev_cell[i].resize(cols);

	stack< pair<int, int>> s;	//스택 s 선언
	s.emplace(make_pair(start.first, start.second)); //스택 s에 시작좌표를 넣는다.
	visited[start.first][start.second] = true;	//시작 좌표를 방문 표시 해준다.

	bool flag = false;	//목적지에 도달했는지 확인하기 위한 flag를 초기화 해준다.

	while (s.size() && !flag) {	//스택 s가 비었거나 flag가 1이면 종료
		y = s.top().first;	//y에 스택 s의 top에 있는 y좌표를 저장
		x = s.top().second;	//x에 스택 s의 top에 있는 x좌표를 저장
		s.pop();	//스택 s를 pop

		for (int d = 0; d < 4; d++) {
			//상하좌우에 있는 cell을 검사 
			if (destination.first - start.first > 0 && destination.second - start.second > 0) {	//출발지가 목적지보다 왼쪽위에 있을경우(1p의 경우)
				nx = x + dx[d];
				ny = y + dy[d];
			}
			else {	//출발지가 목적지보다 오른쪽아래에 잇을경우(2p의 경우)
				nx = x + dx[3 - d];
				ny = y + dy[3 - d];
			}

			if (visited[ny][nx])	//방문한 이력이 있으면 넘어간다.
				continue;
			if (field[ny][nx] != ' ')	//벽으로 막혀있으면 넘어간다.
				continue;

			visited[ny][nx] = true;	//위의 조건에 해당되지 않는다면 방문표시
			s.emplace(make_pair(ny, nx));	//스택 s에 넣어준다.
			prev_cell[ny][nx] = make_pair(y, x);	//현재 좌표 이전 위치를 저장

			if (nx == destination.second && ny == destination.first) {
				flag = true;	//현재 좌표가 목적지에 도달했다면 flag를 세팅하고 break
				break;
			}
		}
	}

	if (flag) {//flag가 세팅되었다면
		int x = destination.second;
		int y = destination.first;
		shortest[y][x] = true; //목적지를 표시

		while (x != start.second || y != start.first) {	//x,y좌표가 시작점에 도달하면 종료
			//이전 좌표를 px,py에 저장
			int px = prev_cell[y][x].second;
			int py = prev_cell[y][x].first;
			shortest[py][px] = true;	//최단거리의 좌표를 업데이트해준다.
			x = px, y = py;	//x,y를 px,py로 바꾸어주어 계속 이전 위치를 찾아가게 해준다.
		}
	}

	return 0;
}

void ofApp::route_draw(pair<int, int> start, pair<int, int> destination) {	//최단거리를 그려주는 함수
	for (int i = 0; i < rows; i++) {
		double y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		y *= size;	//y값을 계산해주고 size를 곱해준다.
		for (int j = 0; j < cols; j++) {
			double x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
				(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
			x *= size;	//x값을 계산해주고 size를 곱해준다.
			if (shortest[i][j]) { //해당 좌표가 최단거리 좌표에 해당하면 그려준다.
				ofSetColor(200);	//최단거리를 회색으로 표시
				if ((i == start.first && j == start.second) || (i == destination.first && j == destination.second)) {
					ofSetColor(ofColor::green);	//출발점과 도착점을 초록색으로 표시
				}
				if ((i % 2) && (j % 2)) {	//i,j가 홀수 이면
					//정사각형을 그려준다.
					ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				}
				else if (i % 2) {	//i만 홀수 이면
					//'-'모양의 직사각형을 그려준다.
					ofDrawRectangle(x - CELL_SIZE * size + WALL_WIDTH * size, y, CELL_SIZE*size, WALL_WIDTH*size);
				}
				else {	//그 외 '|'모양의 직사각형을 그려준다.
					ofDrawRectangle(x, y - CELL_SIZE * size + WALL_WIDTH * size, WALL_WIDTH*size, CELL_SIZE*size);
				}
			}
		}
	}
}

void ofApp::maze_draw() {	//미로를 그려주는 함수
	int i, j;
	for (int i = 0; i < rows; i++) {
		int y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2);
		y *= size;	//y값을 계산해주고 size를 곱해준다
		for (int j = 0; j < cols; j++) {
			int x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2);
			x *= size;	//x값을 계산해주고 size를 곱해준다.
			ofSetColor(ofColor::white);	//미로를 흰색으로 그려준다.

			switch (field[i][j]) {
			case '+':	//'+'이면 정사각형을 그려준다.
				ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				break;
			case '-':	//'-'이면 좌우로 길쭉한 모양의 직사각형을 그려준다.
				ofDrawRectangle(x, y, CELL_SIZE*size, WALL_WIDTH*size);
				break;
			case '|':	//'|'이면 상하로 길쭉한 모양의 직사각형을 그려준다.
				ofDrawRectangle(x, y, WALL_WIDTH*size, CELL_SIZE*size);
				break;
			}
		}
	}
}

void ofApp::ball_set() {	//ball의 위치를 세팅하는 함수
	if (ball_1p_flag) {	//1인용 게임을 실행했을때
		ball_1p = make_pair(1, 1);	//시작지점을 (1(=y),1(=x))로 설정해준다.
		start1 = make_pair(1, 1);	//시작지점을 기억해준다.
		dest = make_pair(rows - 2, cols - 2);	//목적지를 (rows-2(=y),cols-2(=x))로 설정해준다.
	}
	if (ball_2p_flag) {	//2인용 게임을 실행했을때
		ball_1p = make_pair(1, 1);	//ball_1p의 시작지점을 (1(=y),1(=x))로 설정해준다.
		start1 = make_pair(1, 1);	//ball_1p의 시작지점을 기억해준다.
		ball_2p = make_pair(rows - 2, cols - 2);	//ball_2p의 시작지점을 (rows-2(=y),cols-2(=x))로 설정해준다.
		start2 = make_pair(rows - 2, cols - 2);		//ball_2p의 시작지점을 기억해준다.

		//2인용게임에서 목적지를 ball_1p와 ball_2p의 중간지점으로 설정해준다.
		int y = (rows - 1) / 2;
		int x = (cols - 1) / 2;
		if (x % 2 == 0)
			x--;
		if (y % 2 == 0)
			y++;
		dest = make_pair(y, x);	//목적지를 저장해준다.
	}
}

void ofApp::ball_draw() {//ball의 위치를 업데이트 하며 그려주고 게임이 끝났을때 종료메세지를 띄어주는 함수이다.

	if (ball_1p_flag) {	//1인용 게임일 때
		//i,j에 목적지의 좌표를 저장한다.
		int i = dest.first;
		int j = dest.second;
		//미로에 표시될 목적지의 실제 좌표를 계산해준다.
		double dy = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dy *= size;
		double dx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dx *= size;
		//목적지의 색깔을 노란색으로 설정해주고 원으로 표시해준다.
		ofSetColor(ofColor::yellow);
		ofDrawCircle(dx, dy, CELL_SIZE*size / 3);

		//i,j에 ball_1p의 좌표를 저장한다.
		i = ball_1p.first;
		j = ball_1p.second;
		//미로에 표시될 ball_1p의 실제 좌표를 계산해준다.
		double by = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		by *= size;
		double bx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		bx *= size;
		//ball_1p의 색깔을 빨간색으로 설정해주고 원으로 표시해준다.
		ofSetColor(ofColor::red);
		ofDrawCircle(bx, by, CELL_SIZE*size / 3);


		if (bx == dx && by == dy) {	//ball_1p가 목적지에 도착했다면
			game1_end = true;	//end flag를 true로 설정
			game_end();
		}

	}
	if (ball_2p_flag) {	//2인용 게임일 때
		//i,j에 목적지의 좌표를 저장한다.
		int i = dest.first;
		int j = dest.second;
		//미로에 표시될 목적지의 실제 좌표를 계산해준다.
		double dy = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dy *= size;
		double dx = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		dx *= size;
		//목적지의 색깔을 노란색으로 설정해주고 원으로 표시해준다.
		ofSetColor(ofColor::yellow);
		ofDrawCircle(dx, dy, CELL_SIZE*size / 3);

		//i,j에 ball_1p의 좌표를 저장한다.
		i = ball_1p.first;
		j = ball_1p.second;
		//미로에 표시될 ball_1p의 실제 좌표를 계산해준다.
		double b1y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b1y *= size;
		double b1x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b1x *= size;
		//ball_1p의 색깔을 빨간색으로 설정해주고 원으로 표시해준다.
		ofSetColor(ofColor::red);
		ofDrawCircle(b1x, b1y, CELL_SIZE*size / 3);

		//i,j에 ball_2p의 좌표를 저장한다.
		i = ball_2p.first;
		j = ball_2p.second;
		//미로에 표시될 ball_2p의 실제 좌표를 계산해준다.
		double b2y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b2y *= size;
		double b2x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		b2x *= size;
		//ball_2p의 색깔을 빨간색으로 설정해주고 원으로 표시해준다.
		ofSetColor(ofColor::blue);
		ofDrawCircle(b2x, b2y, CELL_SIZE*size / 3);


		if (b1x == dx && b1y == dy) {	//ball_1p가 목적지에 도착했다면
			//end flag를 true로 설정
			game2_end = true;
			game2_end_1 = true;
			game_end();
		}
		if (b2x == dx && b2y == dy) {	//ball_2p가 목적지에 도착했다면
			//end flag를 true로 설정
			game2_end = true;
			game2_end_2 = true;
			game_end();
		}
	}
}
void ofApp::game_end() {

	if (game1_end) {//1인용게임 clear 했을 때
		DFS(start1, dest);	//DFS를 활용하여 ball_1p의 시작지점에서 목적지까지 최단경로를 구해준다.
		route_draw(start1, dest);	//최단경로를 그려준다.
		ofSetColor(ofColor::white);	//하얀색으로 설정
		clear.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);//clear메세지를 띄어준다.
	}
	else if (game2_end) {//2인용게임 clear 했을 때
		if (game2_end_1) {//1p가 이겼을 때
			DFS(start1, dest);	//DFS를 활용하여 ball_1p의 시작지점에서 목적지까지 최단경로를 구해준다.
			route_draw(start1, dest);	//최단경로를 그려준다.
			DFS(start2, dest);	//DFS를 활용하여 ball_2p의 시작지점에서 목적지까지 최단경로를 구해준다.
			route_draw(start2, dest);	//최단경로를 그려준다.
			ofSetColor(ofColor::white);	//하얀색으로 설정
			player1.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);	//'player 1 win!'메세지를 띄어준다.


		}
		else if (game2_end_2) {//2p가 이겼을 때
			DFS(start1, dest);	//DFS를 활용하여 ball_1p의 시작지점에서 목적지까지 최단경로를 구해준다.
			route_draw(start1, dest);	//최단경로를 그려준다.
			DFS(start2, dest);	//DFS를 활용하여 ball_2p의 시작지점에서 목적지까지 최단경로를 구해준다.
			route_draw(start2, dest);	//최단경로를 그려준다.
			ofSetColor(ofColor::white);		//하얀색으로 설정
			player2.draw(ofGetHeight() / 3, ofGetHeight() / 3, ofGetWidth() / 4, ofGetHeight() / 4);	//'player 2 win!'메세지를 띄어준다.
		}
	}
}