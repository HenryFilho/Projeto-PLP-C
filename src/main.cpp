#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>

#define vertical 40
#define horizontal 200
#define DOWN 115
#define UP 119
#define LEFT 97
#define RIGHT 100

typedef struct Player {
	int x, y; // Posição na tela
	int height, width; // Altura e largura
	int MAX_SPEED;
	int yVelocity;
	int pontos;

} Player;

typedef struct Platform {
    int X;
    int Y;
    char symbol;

} Platform;

void PlayerInit(Player *player);
void PlatformInit(Platform *platform);
void PlayerMove(Player *player, int *vel);
void gotoxy(int,int);
void setBorders();
void drawPlayer(Player *player);
void erasePlayer(Player *player);
void movePlayer(Player *player, int vel);
//void debug(char[] msg);

int main() {

	Player player;

	PlayerInit(&player);
	setBorders();
	drawPlayer(&player);
	setBorders();

	while(1) {
		//setBorders();
		erasePlayer(&player);
		movePlayer(&player, -1);
		usleep(250000);
		drawPlayer(&player);
		//setBorders();
	}
	return 0;
}

void PlayerInit(Player *player) {
	player->height = 3;
	player->width = 3;
	player->x = (int) (horizontal/2) - (3);
	player->y = (int) vertical - 3;
	player->MAX_SPEED = 100000;
}

void erasePlayer(Player *player) {
	for (int i = player->x; i < player->x + 5; i++) {
		for (int j = player->y; j < player->y + 3; j++) {
			gotoxy(i, j);
			printf("#");
		}
	}
}

void movePlayer(Player *player, int vel) {
	player->y += vel;
}

void drawPlayer(Player *player) {
	char playerFrame[10][10] = {
		"  o  ",
		"/|_|\\",
		" / \\ "
	};

	for (int i = player->x; i < player->x + 5; i++) {
		for (int j = player->y; j < player->y + 3; j++) {
			gotoxy(i, j);
			printf("%c", playerFrame[j-player->y][i-player->x]);
		}
	}
}

void gotoxy(int x,int y) {
    printf("%c[%d;%dH",0x1B, y, x);
}

void setBorders() {
    int i;

    for (i = 0; i < vertical; i++) {
        gotoxy(0,i);
        printf("@");
        gotoxy(horizontal,i);
        printf("@");
    }

    for (i = 0; i < horizontal; i++) {
      gotoxy(i,0);
      printf("X");
      gotoxy(i,vertical);
      printf("X");
    }
}
/**
void debug(char[] msg) {
	printf(stderr, msg);
}
*/
