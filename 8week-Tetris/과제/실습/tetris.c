#include "tetris.h"

static struct sigaction act, oact;

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();
	recRoot = (RecNode *)malloc(sizeof(RecNode));
	recRoot->lv = -1;
	recRoot->score = 0;
	recRoot->f = field;
	constructRecTree(recRoot);

	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_RECOMMENDED_PLAY: recommendedPlay(); break;
		case MENU_EXIT: listfreeAll(&head); destructRecTree(recRoot); exit = 1; break;
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

	for (i = 0; i < VISIBLE_BLOCKS; i++)
		nextBlock[i] = rand() % 7;

	recommend(recRoot,0);

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
	int i, j;
	int preX, preY, preR;
	preX = blockX;
	preY = blockY;
	preR = blockRotate;
	int shad;

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch (command) {
	case KEY_UP:
		preR = (preR + 3) % 4;
		break;
	case KEY_DOWN:
		preY--;
		break;
	case KEY_RIGHT:
		preX--;
		break;
	case KEY_LEFT:
		preX++;
		break;
	default:
		break;
	}
	shad = preY;
	while (shad < HEIGHT&&CheckToMove(f, currentBlock, preR, shad + 1, preX))
		shad++;

	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][preR][i][j])
			{
				if (i + preY >= 0) {
					move(i + preY + 1, j + preX + 1);
					printw(".");
				}
				if (i + shad >= 0)
				{
					move(i + shad + 1, j + preX + 1);
					printw(".");
				}
			}

		}
	}
	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
	int i;

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		if (blockY == -1) {
			gameOver = TRUE;
		}
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);

		for (i = 0; i < VISIBLE_BLOCKS - 1; i++)
			nextBlock[i] = nextBlock[i + 1];

		nextBlock[VISIBLE_BLOCKS - 1] = rand() % 7;

		recommend(recRoot,0);
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		DrawNextBlock(nextBlock);
		DrawField();

		PrintScore(score);
	}
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	side_touched = 0;
	int i, j;
	int touched = 0;
	for (i = 0; i < BLOCK_HEIGHT; i++) {
		for (j = 0; j < BLOCK_WIDTH; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if (f[blockY + i + 1][blockX + j] == 1 || blockY + i + 1 >= HEIGHT)
					touched++;

				if (blockX + j + 1 < WIDTH &&  f[blockY + i][blockX + j + 1] == 1)
					side_touched++;

				if (blockX + j - 1 >= 0 && f[blockY + i][blockX + j - 1] == 1)
					side_touched++;
			}
		}
	}
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	int i, j, k, l;
	int flag;
	int cnt = 0;
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

		}
	}
	return cnt * cnt * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	while (y < HEIGHT&&CheckToMove(field, blockID, blockRotate, y + 1, x))
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
	char name[NAMELEN];
	int delrank = 0;
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
		break;
	case '2':
		echo();
		printw("input the name: ");
		scanw("%s", name);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");

		for (cur = head; cur != NULL; cur = cur->next) {
			if (!strcmp(cur->name, name)) {
				count++;
				printw(" %-17s| %-10d\n", cur->name, cur->score);
			};
		}
		if (count == 0) {
			printw("\nsearch failure: no name in the list\n");
		}
		break;
	case '3':
		echo();
		printw("input the rank: ");
		scanw("%d", &delrank);
		noecho();
		if (delrank <= 0 || delrank > ranknum || ranknum == 0 || head == NULL) {
			printw("\nsearch failure: the rank not in the list\n");
		}
		else {
			deleterank(delrank, &head);
		}

		break;
	default:
		break;
	}
	getch();

}

void writeRankFile() {
	int i;
	FILE* fp = fopen("rank.txt", "w");
	if (ranknum != 0) {
		fprintf(fp, "%d\n", ranknum);
		node* cur = head;
		while (cur != NULL) {
			fprintf(fp, "%s %d\n", cur->name, cur->score);
			cur = cur->next;
		}
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

void DrawRecommend() {
	DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
}

int recommend(RecNode *root, int lv) {//추천시스템
	int max = 0;
	int i = 0, j, k, r, x, y;
	int tmp;
	RecNode**c = root->c;

	if (lv > VISIBLE_BLOCKS - 1)
		return 0;

	for (r = 0; r < NUM_OF_ROTATE; r++)
	{
		if (nextBlock[lv] == 4 && r > 0)
			break;

		else if (r > 1 && (nextBlock[lv] == 0 || nextBlock[lv] == 5 || nextBlock[lv] == 6))
			break;

		for (x = -3; x < WIDTH; x++)
		{
			y = -1;

			if (CheckToMove(root->f, nextBlock[lv], r, y, x) == 1)
			{

				for (j = 0; j < HEIGHT; j++)
					for (k = 0; k < WIDTH; k++)
						c[i]->f[j][k] = root->f[j][k];

				while (CheckToMove(c[i]->f, nextBlock[lv], r, y + 1, x))
					y++;

				c[i]->curBlockID = nextBlock[lv];
				c[i]->recBlockRotate = r;
				c[i]->recBlockX = x;
				c[i]->recBlockY = y;

				c[i]->score = root->score + evalState(lv, c[i]->f, r, y, x);
				if (c[i]->score - root->score <= 10)
					continue;

				if (lv == VISIBLE_BLOCKS - 1)
					max = c[i]->score;
				else
				{
					tmp = recommend(c[i],lv+1);

					if (max < tmp)
					{
						max = tmp;

						if (lv == 0)
						{
							recRoot->curBlockID = nextBlock[lv];
							recRoot->recBlockRotate = r;
							recRoot->recBlockX = x;
							recRoot->recBlockY = y;

						}
					}

					else if (max == tmp && lv == 0 && recRoot->recBlockY < y)
					{
						max = tmp;

						if (lv == 0)
						{
							recRoot->curBlockID = nextBlock[lv];
							recRoot->recBlockRotate = r;
							recRoot->recBlockX = x;
							recRoot->recBlockY = y;

						}

					}
				}
				i++;
			}
		}

	}

	recommendX = recRoot->recBlockX;
	recommendY = recRoot->recBlockY;
	recommendR = recRoot->recBlockRotate;

	return max;
}

void recommendedPlay() {
	// user code
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
	DrawRecommend();
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

void deleterank(int rank, node** head) {
	int count = 0;
	node* cur = *head;
	node* pre;
	while (cur != NULL) {
		count++;
		if (count == rank) {
			if (count == 1) {
				pre = cur;
				cur = cur->next;
				free(pre);
				*head = cur;
				ranknum--;
				printw("\nresult: the rank deleted\n");
				writeRankFile();
				return;
			}
			else {
				pre->next = cur->next;
				free(cur);
				ranknum--;
				printw("\nresult: the rank deleted\n");
				writeRankFile();
				return;
			}
		}
		pre = cur;
		cur = cur->next;
	}
}

void listfreeAll(node** head) {
	node* cur = *head;
	node* pre;
	if (cur) {
		while (cur->next != NULL) {
			pre = cur;
			cur = cur->next;
			free(pre);
		}
		free(cur);
	}
}

void constructRecTree(RecNode *root) {
	int i, h;
	RecNode **c = root->c;
	for (i = 0; i < CHILDREN_MAX; ++i) {
		c[i] = (RecNode *)malloc(sizeof(RecNode));
		c[i]->lv = root->lv + 1;
		c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
		if (c[i]->lv < VISIBLE_BLOCKS - 1) {
			constructRecTree(c[i]);
		}
	}
}

void destructRecTree(RecNode *root) {
	int i;
	for (i = 0; i < CHILDREN_MAX; ++i) {
		if (root->c[i]) destructRecTree(root->c[i]);
	}
	free(root);
}

int evalState(int lv, char f[HEIGHT][WIDTH], int r, int y, int x) {
	return AddBlockToField(f, nextBlock[lv], r, y, x) + 5 * DeleteLine(f) + 5 * y + 2 * side_touched;
}