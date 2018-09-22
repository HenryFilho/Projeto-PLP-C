#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ncurses.h> // Biblioteca responsável pelo mvprintw
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctype.h>

#define VERTICAL 36
#define HORIZONTAL 70
#define DOWN 115
#define UP 119
#define LEFT 97
#define RIGHT 100
#define GRAVITY 3

int score = 0;

typedef struct Player {
	int x, y; // Posição na tela
	int height, width; // Altura e largura
	int MAX_SPEED;
	int yVelocity;
	int pontos;

} Player;

typedef struct Platform {
    int x, y;
	int length;
	int CONSTRUCTION_SPEED;
    char symbol[2];
} Platform;

//Game
void GameInit(Player *player, Platform *latform);
void PlatformInit(Platform *platform);
void spawnPlatform(Platform *platform);
void drawPlatform(Platform *platform, int sideSpawn, int direction);
// Player
void PlayerInit(Player *player);
void drawPlayer(Player *player);
void erasePlayer(Player *player);
void movePlayer(Player *player);
void playerJump(Player *);
// Util
// void debug(const char * msg);
void setBorders();

int main() {
	initscr();
	keypad(stdscr, TRUE);
	noecho();
 	curs_set(FALSE); //Deixa o cursor invisível

	Player player;
	Platform platform;
	GameInit(&player, &platform);
	char cmd;
	while(1) {
		cmd = toupper(getchar());
		switch (cmd) {
			case 'P':
				spawnPlatform(&platform);
				break;
			case 'W':
			case UP:
				movePlayer(&player);
				break;
			case 'Q':
				endwin();
				exit(0);
		}
	}
	return 0;
}

void GameInit (Player *player, Platform *platform) {
	PlayerInit(player);
	PlatformInit(platform);
	setBorders();
	drawPlayer(player);
	refresh();
}

void PlatformInit(Platform *platform) {
	platform->x = HORIZONTAL;
	platform->y = VERTICAL;
	platform->length = (int) (HORIZONTAL/3) + 20;
	strcpy(platform->symbol, "=");
	platform->CONSTRUCTION_SPEED = 30000;
}

void PlayerInit(Player *player) {
	player->height = 3;
	player->width = 3;
	player->x = (int) (HORIZONTAL/2) - 2;
	player->y = (int) VERTICAL - 3;
	player->MAX_SPEED = 85500;	
}

void spawnPlatform(Platform *platform) {
	int initSpawn = rand() % 18;
	if (rand() % 2 == 1)
		drawPlatform(platform, HORIZONTAL - initSpawn, -1);
	else
		drawPlatform(platform, initSpawn, 1);	
}

void drawPlatform(Platform *platform, int sideSpawn, int direction) {
	int j;
	for (j = 1; j <= platform->length + (rand () % 5); j++) {
		int y = sideSpawn + (j * direction );
		mvprintw(VERTICAL - score - 1, y, platform->symbol);
		refresh();
		usleep(platform->CONSTRUCTION_SPEED);
	}	
	score++;
}

void drawPlayer(Player *player) {
	mvprintw(player->y, player->x, "  o  ");
	mvprintw(1 + player->y, player->x, "/|_|\\");
	mvprintw(2 + player->y, player->x, " / \\ ");
	refresh();
}

void erasePlayer(Player *player) {
	mvprintw(player->y, player->x, "     ");
	mvprintw(1 + player->y, player->x, "     ");
	mvprintw(2 + player->y, player->x, "     ");
}

void movePlayer(Player *player) {
	int i;
    for(i = 1; i <= GRAVITY; i++) {
        erasePlayer(player);
		player->y -= i;
		drawPlayer(player);
        usleep(player->MAX_SPEED);
	}
	for(i = 1; i <= GRAVITY; i++) {
		erasePlayer(player);
		player->y += i;
		drawPlayer(player);
        usleep(player->MAX_SPEED);
    }
}

void setBorders() {
	for (int j = 0; j < HORIZONTAL + 1; j++) {
		if(j + 12 <= HORIZONTAL)
			mvprintw(0, j + 14, "X");
		mvprintw(VERTICAL, j, "X");
	}
	mvprintw(0, 1, " Pontuação: %06d ", score);
	for (int j = 0; j < VERTICAL; j++) {
		mvprintw(j, 0, "X");
		mvprintw(j, HORIZONTAL, "X");
	}
}

// void debug(const char * msg) {
// 	fprintf(stderr, msg);
// }


// int kbhit(void) {
//   struct termios oldt, newt;
//   int ch;
//   int oldf;

//   tcgetattr(STDIN_FILENO, &oldt);
//   newt = oldt;
//   newt.c_lflag &= ~(ICANON | ECHO);
//   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//   oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//   fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//   ch = getchar();

//   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//   fcntl(STDIN_FILENO, F_SETFL, oldf);

//   if(ch != EOF) {
//     ungetc(ch, stdin);
//     return 1;
//   }
//   return 0;
// }