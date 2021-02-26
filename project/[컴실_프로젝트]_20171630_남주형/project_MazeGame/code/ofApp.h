/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

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
*/

#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key); // Traps escape key if exit disabled
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool readFile(); //파일을 읽어 정보를 저장하는 함수
	void freeMemory(); //메모리 할당해제
	bool DFS(pair<int,int> start, pair<int, int> destination); //DFS를 통해 최단경로를 구하는 함수
	void route_draw(pair<int,int> start, pair<int, int> destination); //최단경로를 그려주는 함수
	void maze_draw();	//미로를 그려주는 함수
	void ball_draw(); //ball을 그려주는 함수
	void game_end();

	ofxWinMenu * menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function

	ofTrueTypeFont myFont;
	float windowWidth, windowHeight;	//화면의 너비와 높이를 저장하는 변수
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	bool bFullscreen;	//전체화면을 실행하는 flag
	int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
	bool ball_1p_flag;	//1인용을 선택했는지 확인하는 flag
	bool ball_2p_flag;	//2인용을 선택했는지 확인하는 flag
	bool game1_end;	//1인용 게임 종료 flag
	bool game2_end;	//2인용 게임 종료 flag
	bool game2_end_1;	//2인용 게임 종료 flag(player1win)
	bool game2_end_2;	//2인용 게임 종료 flag(player2win)
	void doFullScreen(bool bFull);	//전체화면을 실행하는 함수

	void ball_set();	//ball의 첫 위치를 세팅하는 함수
	vector<vector<char>> field;	//미로의 모양을 저장하는 변수
	vector<vector<bool>> visited, shortest;	//DFS에서 사용되는 변수
	int cols, rows;	//열과 행의 수를 저장하는 변수
	int dx[4] = { 0, -1, 0, 1 };	//dx와 dy는 주변 cell을 검사하기 위한 변수이다.
	int dy[4] = { -1, 0, 1, 0 };
	double size;	//미로의 size를 조절하기 위한 변수
	pair<int, int> start1, start2, dest;	//차례로 ball_1p, ball_2p의 시작지점을 저장하기 위한 변수,목적지를 저장하기 위한 변수
	pair<int, int> ball_1p, ball_2p;	//ball_1p,ball_2p의 위치를 저장하는 변수

	ofImage clear;		//clear이미지
	ofImage player1;	//player1win이미지
	ofImage player2;	//player2win이미지
};