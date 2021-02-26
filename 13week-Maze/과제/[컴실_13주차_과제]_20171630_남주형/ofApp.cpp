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
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	route_flag = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

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

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS", false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
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
			freeMemory();
			route_flag = false;
		}

		readFile();
	}
	if (title == "Exit") {
		cout << "Quit" << endl;
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			route_flag = true;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		bShowInfo = bChecked;	// Flag is used elsewhere in Draw()
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			BFS();
			route_flag = true;
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
	size = (double)ofGetHeight() / (double)(CELL_SIZE * ((rows) / 2) + WALL_WIDTH * (rows - (rows) / 2));
	if (size > (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2)))
		size = (double)ofGetWidth() / (double)(CELL_SIZE * ((cols) / 2) + WALL_WIDTH * (cols - (cols) / 2));
}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	ofSetLineWidth(7);
	for (int i = 0; i < rows; i++) {
		int y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2);
		y *= size;
		for (int j = 0; j < cols; j++) {
			int x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2);
			x *= size;
			ofSetColor(ofColor::black);

			switch (field[i][j]) {
			case '+':
				ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				break;
			case '-':
				ofDrawRectangle(x, y, CELL_SIZE*size, WALL_WIDTH*size);
				break;
			case '|':
				ofDrawRectangle(x, y, WALL_WIDTH*size, CELL_SIZE*size);
				break;
			}
		}
	}




	if (route_flag)
	{
		ofSetLineWidth(5);
		if (isOpen)
			route_draw();
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
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
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			cout << "Quit" << endl;
			freeMemory();
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
	if (key == 'q' || key == 'Q') {
		cout << "Quit" << endl;
		freeMemory();
		ofExit();
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
bool ofApp::readFile()
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
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

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
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
					string line = *it;
					vector<char> words(line.begin(), line.end());
					field.emplace_back(words);
				}
			}
			rows = field.size();
			cols = field[0].size();

			checked.resize(rows);
			visited.resize(rows);
			shortest.resize(rows);

			for (int i = 0; i < rows; i++)
				checked[i].resize(cols);
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
void ofApp::freeMemory() {
	cols = 0;
	rows = 0;
	field.clear();
	checked.clear();
	visited.clear();
	shortest.clear();
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	for (int i = 0; i < rows; i++)
		fill(checked[i].begin(), checked[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(visited[i].begin(), visited[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(shortest[i].begin(), shortest[i].end(), false);

	dest_y = rows - 2;
	dest_x = cols - 2;

	vector< vector< pair<int, int>>> prev_cell;
	prev_cell.resize(rows);
	for (int i = 0; i < rows; i++)
		prev_cell[i].resize(cols);

	stack< pair<int, int>> s;
	s.emplace(make_pair(1, 1));
	visited[1][1] = true;

	bool flag = false;

	while (s.size() && !flag) {
		int y = s.top().first;
		int x = s.top().second;
		checked[y][x] = true;
		s.pop();

		for (int d = 0; d < 4; d++) {
			int ny = y + dy[d];
			int nx = x + dx[d];

			if (visited[ny][nx])
				continue;
			if (field[ny][nx] != ' ')
				continue;

			visited[ny][nx] = true;
			s.emplace(make_pair(ny, nx));
			prev_cell[ny][nx] = make_pair(y, x);

			if (ny == dest_y && nx == dest_x) {
				flag = true;
				break;
			}
		}
	}

	if (flag) {
		int y = dest_y;
		int x = dest_x;
		shortest[y][x] = true;

		while (y != 1 || x != 1) {
			int py = prev_cell[y][x].first;
			int px = prev_cell[y][x].second;
			shortest[py][px] = true;
			y = py, x = px;
		}
	}


	return 0;
}

bool ofApp::BFS()
{
	for (int i = 0; i < rows; i++)
		fill(checked[i].begin(), checked[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(visited[i].begin(), visited[i].end(), false);
	for (int i = 0; i < rows; i++)
		fill(shortest[i].begin(), shortest[i].end(), false);

	dest_y = rows - 2;
	dest_x = cols - 2;

	vector< vector< pair<int, int>>> prev_cell;
	prev_cell.resize(rows);
	for (int i = 0; i < rows; i++)
		prev_cell[i].resize(cols);

	queue< pair<int, int>> q;
	q.emplace(make_pair(1, 1));
	visited[1][1] = true;

	bool flag = false;

	while (q.size() && !flag) {
		int y = q.front().first;
		int x = q.front().second;
		checked[y][x] = true;
		q.pop();

		for (int d = 0; d < 4; d++) {
			int ny = y + dy[d];
			int nx = x + dx[d];

			if (visited[ny][nx])
				continue;
			if (field[ny][nx] != ' ')
				continue;

			visited[ny][nx] = true;
			q.emplace(make_pair(ny, nx));
			prev_cell[ny][nx] = make_pair(y, x);

			if (nx == dest_x && ny == dest_y) {
				flag = true;
				break;
			}
		}
	}

	if (flag) {
		int x = dest_x;
		int y = dest_y;
		shortest[y][x] = true;

		while (x != 1 || y != 1) {
			int px = prev_cell[y][x].second;
			int py = prev_cell[y][x].first;
			shortest[py][px] = true;
			x = px, y = py;
		}
	}

	return 0;
}


void ofApp::route_draw() {
	ofSetColor(200);
	for (int i = 0; i < rows; i++) {
		double y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		y *= size;
		for (int j = 0; j < cols; j++) {
			double x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
				(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
			x *= size;
			if (checked[i][j]) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				}
				else if (i % 2) {
					ofDrawRectangle(x - CELL_SIZE * size + WALL_WIDTH * size, y, CELL_SIZE*size, WALL_WIDTH*size);
				}
				else {
					ofDrawRectangle(x, y - CELL_SIZE * size + WALL_WIDTH * size, WALL_WIDTH*size, CELL_SIZE*size);
				}
			}
		}
	}

	for (int i = 0; i < rows; i++) {
		double y = CELL_SIZE * (i / 2) + WALL_WIDTH * (i - i / 2) +
			(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
		y *= size;
		for (int j = 0; j < cols; j++) {
			double x = CELL_SIZE * (j / 2) + WALL_WIDTH * (j - j / 2) +
				(double)CELL_SIZE / 2 - (double)WALL_WIDTH / 2;
			x *= size;
			if (shortest[i][j]) {
				ofSetColor(ofColor::red);
				if ((i == 1 && j == 1) || (i == dest_y && j == dest_x)) {
					ofSetColor(ofColor::green);
				}
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, WALL_WIDTH*size, WALL_WIDTH*size);
				}
				else if (i % 2) {
					ofDrawRectangle(x - CELL_SIZE * size + WALL_WIDTH * size, y, CELL_SIZE*size, WALL_WIDTH*size);
				}
				else {
					ofDrawRectangle(x, y - CELL_SIZE * size + WALL_WIDTH * size, WALL_WIDTH*size, CELL_SIZE*size);
				}
			}
		}
	}
}