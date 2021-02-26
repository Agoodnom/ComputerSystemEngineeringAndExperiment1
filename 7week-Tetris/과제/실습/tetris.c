#include "tetris.h"

static struct sigaction act, oact;

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();

	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: freeAll(&head); exit = 1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	nextBlock[0] = rand() % 7;
	nextBlock[1] = rand() % 7;
	nextBlock[2] = rand() % 7;
	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i, j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);
	DrawBox(9, WIDTH + 10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15, WIDTH + 10);
	printw("SCORE");
	DrawBox(16, WIDTH + 10, 1, 8);
}

int GetCommand() {
	int command;
	command = wgetch(stdscr);
	switch (command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret = 1;
	int drawFlag = 0;
	switch (command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	case ' ':
		while ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	default:
		break;
	}
	if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField() {
	int i, j;
	for (j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++) {
			if (field[j][i] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
	move(17, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int *nextBlock) {
	int i, j;
	for (i = 0; i < 4; i++) {
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[1]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for (i = 0; i < 4; i++) {
		move(10 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}
	}
	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int i, j;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				if (f[i + blockY][j + blockX] == 1) return 0;
				if (i + blockY >= HEIGHT || j + blockX < 0 || j + blockX >= WIDTH) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);

}

void BlockDown(int sig) {

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		if (blockY == -1) {
			gameOver = TRUE;
		}
		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		DrawNextBlock(nextBlock);
		DrawField();

		PrintScore(score);
	}
	timed_out = 0;
}

void AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int i, j;
	int touched = 0;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if (f[blockY + i + 1][blockX + j] == 1 || blockY + i + 1 >= HEIGHT)
					touched++;
			}
		}
	}
	score += touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	int i, j, k, l;
	int flag;
	int score, cnt = 0;
	for (j = 0; j < HEIGHT; j++) {
		flag = 0;
		for (i = 0; i < WIDTH; i++) {
			if (f[j][i] != 1) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			cnt++;
			for (k = j; k > 0; k--) {
				for (l = 0; l < WIDTH; l++) {
					f[k][l] = f[k - 1][l];
				}
			}
			for (l = 0; l < WIDTH; l++) {
				f[0][l] = 0;
			}
			cnt++;

		}
	}
	score = cnt * cnt * 100;
	return score;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	while (y < HEIGHT&&CheckToMove(field, blockID, blockRotate, y + 1, x) == 1)
		y++;

	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList() {
	FILE *fp = fopen("rank.txt", "r");
	int i;
	int score;
	char name[NAMELEN];

	if (fp == NULL) {
		return;
	}
	else {
		fscanf(fp, "%d\n", &ranknum);

		for (i = 0; i < ranknum; i++) {
			fscanf(fp, "%s", name);
			fscanf(fp, "%d\n", &score);
			addrank(score, name, &head);
		}
	}
	fclose(fp);
}

void rank() {
	// user code
	int x = 1, y = ranknum;
	int count = 0;
	int i;
	int command;

	node* cur;

	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	command = wgetch(stdscr);

	switch (command) {
	case '1':
		echo();
		printw("X : ");
		scanw("%d", &x);
		printw("Y : ");
		scanw("%d", &y);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");

		if (y < x || y <= 0 || x <= 0 || x>ranknum || y > ranknum || ranknum == 0 || head == NULL) {
			printw("\nsearch failure: no rank in the list\n");
		}
		else {
			for (cur = head; cur != NULL; cur = cur->next) {
				count++;
				if (count >= x && count <= y) {
					printw(" %-17s| %-10d\n", cur->name, cur->score);
				};
			}
			
		}


		getch();
		break;
	default:
		break;
	}

	
}

void writeRankFile() {
	int i;
	FILE* fp = fopen("rank.txt", "w");
	fprintf(fp, "%d\n", ranknum);
	node* cur = head;
	while (cur != NULL) {
		fprintf(fp, "%s %d\n", cur->name, cur->score);
		cur = cur->next;
	}
	fclose(fp);
}

void newRank(int score) {
	char name[NAMELEN];
	clear();
	echo();
	printw("your name: ");
	scanw("%s", name);
	noecho();
	ranknum++;
	addrank(score, name, &head);
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	// user code
}

int recommend(RecNode *root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay() {
	// user code
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

void addrank(int score, char name[NAMELEN], node** head) {
	node* newnode;
	node* cur = *head;
	node* pre;

	newnode = (node*)malloc(sizeof(node));
	newnode->score = score;
	strcpy(newnode->name, name);
	newnode->next = NULL;

	if (cur == NULL) {
		*head = newnode;
	}
	else {
		if (cur->score < score) {
			newnode->next = cur;
			*head = newnode;
		}
		else {
			while ((cur->score >= score) && (cur->next != NULL)) {
				pre = cur;
				cur = cur->next;
			}
			if (cur->score < score&&cur->next != NULL) {
				newnode->next = cur;
				pre->next = newnode;
			}
			else {
				cur->next = newnode;
			}
		}
	}
}

void freeAll(node** head) {
	node* cur = *head;
	node* pre;

	while (cur->next != NULL) {
		pre = cur;
		cur = cur->next;
		free(pre);
	}
	free(cur);
}
