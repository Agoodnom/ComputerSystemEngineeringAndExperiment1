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
	bool readFile(); //������ �о� ������ �����ϴ� �Լ�
	void freeMemory(); //�޸� �Ҵ�����
	bool DFS(pair<int,int> start, pair<int, int> destination); //DFS�� ���� �ִܰ�θ� ���ϴ� �Լ�
	void route_draw(pair<int,int> start, pair<int, int> destination); //�ִܰ�θ� �׷��ִ� �Լ�
	void maze_draw();	//�̷θ� �׷��ִ� �Լ�
	void ball_draw(); //ball�� �׷��ִ� �Լ�
	void game_end();

	ofxWinMenu * menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function

	ofTrueTypeFont myFont;
	float windowWidth, windowHeight;	//ȭ���� �ʺ�� ���̸� �����ϴ� ����
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	bool bFullscreen;	//��üȭ���� �����ϴ� flag
	int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
	bool ball_1p_flag;	//1�ο��� �����ߴ��� Ȯ���ϴ� flag
	bool ball_2p_flag;	//2�ο��� �����ߴ��� Ȯ���ϴ� flag
	bool game1_end;	//1�ο� ���� ���� flag
	bool game2_end;	//2�ο� ���� ���� flag
	bool game2_end_1;	//2�ο� ���� ���� flag(player1win)
	bool game2_end_2;	//2�ο� ���� ���� flag(player2win)
	void doFullScreen(bool bFull);	//��üȭ���� �����ϴ� �Լ�

	void ball_set();	//ball�� ù ��ġ�� �����ϴ� �Լ�
	vector<vector<char>> field;	//�̷��� ����� �����ϴ� ����
	vector<vector<bool>> visited, shortest;	//DFS���� ���Ǵ� ����
	int cols, rows;	//���� ���� ���� �����ϴ� ����
	int dx[4] = { 0, -1, 0, 1 };	//dx�� dy�� �ֺ� cell�� �˻��ϱ� ���� �����̴�.
	int dy[4] = { -1, 0, 1, 0 };
	double size;	//�̷��� size�� �����ϱ� ���� ����
	pair<int, int> start1, start2, dest;	//���ʷ� ball_1p, ball_2p�� ���������� �����ϱ� ���� ����,�������� �����ϱ� ���� ����
	pair<int, int> ball_1p, ball_2p;	//ball_1p,ball_2p�� ��ġ�� �����ϴ� ����

	ofImage clear;		//clear�̹���
	ofImage player1;	//player1win�̹���
	ofImage player2;	//player2win�̹���
};