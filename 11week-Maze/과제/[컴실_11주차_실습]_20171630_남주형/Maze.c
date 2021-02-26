#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

void draw_outline();
void draw_width();
void draw_height();
void w_connect();
void h_connect();
int *w_bar;
int *h_bar;
int *room;
int n, m;
int cur = 1;
FILE *fp;

int main() {
	int i, j;
	int bar = -1;
	fp = fopen("maze.maz", "wt");

	srand(time(NULL));

	printf("WIDTH: ");
	scanf("%d", &n);
	printf("HEIGHT: ");
	scanf("%d", &m);

	w_bar = (int*)malloc(sizeof(int)*(n));//width_bar"-"
	h_bar = (int*)malloc(sizeof(int)*(n - 1));//height_bar"|"
	room = (int*)malloc(sizeof(int)*n);
	memset(room, 0, sizeof(int)*n);


	//first line
	draw_outline();
	for (i = 0; i < n - 1; i++) {
		h_bar[i] = rand() % 2;
		if (h_bar[i]) {
			for (j = bar + 1; j <= i; j++) {
				room[j] = cur;
			}
			cur++;
			bar = i;
		}
	}

	for (i = bar + 1; i < n; i++) {
		room[i] = cur;
	}
	cur++;
	draw_height();

	//middle lines
	for (i = 1; i < m - 1; i++) {
		w_connect();
		draw_width();
		h_connect();
		draw_height();
	}

	//last line
	w_connect();
	draw_width();

	for (i = 0; i < n - 1; i++) {
		if (room[i] != room[i + 1]) {
			h_bar[i] = 0;
			bar = room[i + 1];
			room[i + 1] = room[i];
			for (j = 0; j < n; j++) {
				if (room[j] == bar)
					room[j] = room[i];
			}
		}
		else
			h_bar[i] = 1;
	}
	draw_height();
	draw_outline();

	fclose(fp);

	return 0;
}


void draw_outline() {
	int i;
	for (i = 0; i < n; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");
}

void draw_width() {
	int i;
	fprintf(fp, "+");

	for (i = 0; i < n; i++) {
		if (w_bar[i])
			fprintf(fp, "-+");

		else
			fprintf(fp, " +");
	}

	fprintf(fp, "\n");
}

void draw_height() {
	int i;
	fprintf(fp, "|");

	for (i = 0; i < n - 1; i++) {
		if (h_bar[i])
			fprintf(fp, " |");

		else
			fprintf(fp, "  ");
	}

	fprintf(fp, " |\n");
}

void w_connect() {//"-"
	int i;
	int roomnum = room[0];
	int bar_flag = 0;

	for (i = 0; i < n; i++) {
		w_bar[i] = rand() % 2;
		if (!w_bar[i])
			bar_flag = 1;

		if ((i < n - 1) && (roomnum != room[i + 1])) {
			if (!bar_flag)
				w_bar[i] = 0;

			bar_flag = 0;
			roomnum = room[i + 1];
		}

		if ((i == n - 1) && (bar_flag == 0))
			w_bar[i] = 0;

		if (w_bar[i]) {
			room[i] = cur;
			cur++;
		}
	}
}
void h_connect() {//"|"
	int i, j;
	int roomnum;

	for (i = 0; i < n - 1; i++) {
		if (room[i] != room[i + 1]) {
			h_bar[i] = rand() % 2;
			if (!h_bar[i]) {
				roomnum = room[i + 1];
				room[i + 1] = room[i];
				for (j = 0; j < n; j++) {
					if (room[j] == roomnum)
						room[j] = room[i];
				}
			}
		}
		else
			h_bar[i] = 1;
	}

}