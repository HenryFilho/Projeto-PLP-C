#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctype.h>
#include <time.h>

#define VERTICAL 37
#define HORIZONTAL 80
#define GRAVITY 1

typedef struct Player {
	int x, y; // Posição na tela	
	int MAX_SPEED;
	int yVelocity;
} Player;

typedef struct Platform {
    int endX, startX, y;
    char symbol[2];
	int direction;
} Platform;

//Game
int game();
int score = 0;
void GameInit(Player *player, Platform *platform);
int gameOver(Player *player, Platform *platform);
//Platform
void PlatformInit(Platform *platform);
void spawnPlatform(Platform *platform);
void drawHeadPlatform(Platform *platform);
void movePlatformHead(Platform *platform);
int finishedConstruction(Platform *platform);
// Player
void PlayerInit(Player *player);
void drawPlayer(Player *player);
void erasePlayer(Player *player);
void movePlayer(Player *player);
void playerJump(Player *);
// Util
void debug(const char * msg);
void setBorders();
int kbhit(void);

int game() {
	Player player;
	Platform platform;
	GameInit(&player, &platform);

	while (1) {
		while (!kbhit()) {
			erasePlayer(&player);
			if (gameOver(&player, &platform) == 1) {
				refresh();
				echo();
 				curs_set(TRUE);		
				endwin();
				return 1;
			}

			movePlayer(&player);
			if (finishedConstruction(&platform))
				spawnPlatform(&platform);
			movePlatformHead(&platform);
			setBorders();
			usleep(player.MAX_SPEED);
			refresh();
		}

		char cmd = toupper(getchar());
		switch (cmd) {
			case ' ':
				playerJump(&player);
				break;
			case 'I':
				player.MAX_SPEED = 100000000;
				break;
			case 'P':
				player.MAX_SPEED = 800000;
				break;
			case 'O':
				player.MAX_SPEED = 80000;
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

	initscr();
	noecho();
 	curs_set(FALSE);

	setBorders();
	mvprintw(0, 2, "Pontuação: %d", score);
	drawPlayer(player);
}

int gameOver(Player *player, Platform *platform) {
	if ((platform->direction > 0 && platform->startX == player->x + 3) ||
	(platform->direction < 0 && platform->startX == player->x + 1)) {
		if (player->y + 2 == platform->y){
			drawPlayer(player);
			mvprintw(4 ,(HORIZONTAL/2) - 6, " VOCE PERDEU", platform->y, player->y + 2);
			mvprintw(2 + player->y, player->x + 1, "x");
			mvprintw(2 + player->y, player->x + 3, "x");
			return 1;
		} else {
			score++;
			return 0;
		}
	}
	return 0;
}

void PlatformInit(Platform *platform) {
	platform->endX = 0;
	platform->startX = 0;
	platform->y = VERTICAL - 4;
	platform->direction = 0;
	strcpy(platform->symbol, "=");
}

void PlayerInit(Player *player) {
	player->x = (int) (HORIZONTAL/2) - 2;
	player->y = (int) VERTICAL - 4;
	player->MAX_SPEED = 100000;
}

void playerJump(Player *player) {
	if(player->y >= VERTICAL - 3 - score)
		player->yVelocity = -3;
}

void drawPlayer(Player *player) {
	mvprintw(player->y, player->x, "  o  ");
	mvprintw(1 + player->y, player->x, "/|_|\\");
	mvprintw(2 + player->y, player->x, " / \\");
}

void erasePlayer(Player *player) {
	mvprintw(player->y, player->x, "     ");
	mvprintw(1 + player->y, player->x, "     ");
	mvprintw(2 + player->y, player->x, "     ");
}

void movePlayer(Player *player) {
	if(player->y >= VERTICAL - 3 - score && player->yVelocity >= 0) {
		player->yVelocity = 0;
	} else {
		player->yVelocity += GRAVITY;
		player->y += player->yVelocity;
	}
	if (player->y > VERTICAL - 3 - score) player->y = VERTICAL - 3 - score;
	drawPlayer(player);
}

void setBorders() {
	for (int j = 0; j < HORIZONTAL + 1; j++) {
		if(j > 11 && j + 10 <= HORIZONTAL)
			mvprintw(0, j + 10, "X");
		mvprintw(VERTICAL, j, "X");
	}
	mvprintw(0, 2, "Pontuação: %d", score);
	for (int j = 0; j <= VERTICAL; j++) {
		mvprintw(j, 0, "X");
		mvprintw(j, HORIZONTAL, "X");
	}
}

void spawnPlatform(Platform *platform) {
	srand(time(NULL));
	int initSpawn = rand() % 18;
	if (rand() % 2 == 0) {
		platform->endX = initSpawn + (HORIZONTAL/2) + 10;
		platform->startX = initSpawn;
		platform->direction = -1;
	} else {
		platform->endX = (HORIZONTAL - initSpawn) - (HORIZONTAL/2) - 10;
		platform->startX = (HORIZONTAL - initSpawn);
		platform->direction = 1;
	}
	platform->y = VERTICAL - score - 1;
}
void movePlatformHead(Platform *platform) {
	mvprintw(platform->y, platform->startX, platform->symbol);
	if (platform->endX != platform->startX)
		platform->startX -= platform->direction;
}

int finishedConstruction(Platform *platform) {
	if (platform->endX == platform->startX)
		return 1;
	return 0;
}

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}