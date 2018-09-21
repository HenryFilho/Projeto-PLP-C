#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>

#define vertical 40
#define horizontal 200
#define DOWN 115
#define UP 119
#define LEFT 97
#define RIGHT 100
#define GRAVITY 2

typedef struct Player {
	int x, y; // Posição na tela
	int height, width; // Altura e largura
	int MAX_SPEED;
	int yVelocity;
	int pontos;

} Player;

typedef struct Platform {
    int x;
    int y;
    char symbol;

} Platform;

void PlayerInit(Player *player);
void PlatformInit(Platform *platform);
void PlayerMove(Player *player, int *vel);
void gotoxy(int, int);
void setBorders();
void drawPlayer(Player *player);
void erasePlayer(Player *player);
void movePlayer(Player *player);
void playerJump(Player *);
void debug(const char * msg);
int kbhit();

unsigned long globalCount = 0;

int main() {

	Player player;

	PlayerInit(&player);

	setBorders();
	drawPlayer(&player);
	setBorders();

	system("clear");
	//system("stty -echo");
	//system ("/bin/stty raw");

	char cmd;

	//while(!(game_over(&player))) {
	while(1) {

			// Enquanto teclado não utilizado, fica neste loop
			while (!kbhit()) {
						 usleep(player.MAX_SPEED);
						 erasePlayer(&player);
						 movePlayer(&player);
						 drawPlayer(&player);
				 		 setBorders();
						 //if (gameOver(&player)) {
						 //	 break;
						 //}
			}

			cmd = toupper(getchar());
			switch (cmd) {
				case ' ':
				case UP:
					playerJump(&player);
					break;
				case 'Q':
				system("clear");
					exit(0);
				default:
					printf("\a");
					break;
			}
 }
	return 0;
}

void playerJump(Player *player) {
	player->yVelocity = -6;
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
			printf(" ");
		}
	}
}

void movePlayer(Player *player) {
	if(player->y >= vertical-3 && player->yVelocity >= 0) {
		player->yVelocity = 0;
	} else {
		player->yVelocity += GRAVITY;
		player->y += player->yVelocity;
	}
	if (player->y > vertical-3) player->y = vertical-3;
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
			//char s[400];
			// sprintf(s, "Imprimi playerFrame[%d][%d] = %c, na posicao i = %d, j = %d\n", j-player->y, i-player->x, playerFrame[j-player->y][i-player->x], i, j);
			// debug(s);
		}
		// char s[400];
		// sprintf(s, "==========================\n");
		// debug(s);
	}
	// char s[400];
	// sprintf(s, "----------------------------\n\n");
	// debug(s);
}

void gotoxy(int x,int y) {
    printf("%c[%d;%dH",0x1B, y, x);
}

void setBorders() {
    int i;

    for (i = 0; i < vertical; i++) {
        gotoxy(0,i);
        printf("X");
        gotoxy(horizontal,i);
        printf("X");
    }

    for (i = 0; i < horizontal; i++) {
      gotoxy(i,0);
      printf("X");
      gotoxy(i,vertical);
      printf("X");
    }
}

void debug(const char * msg) {
	fprintf(stderr, msg);
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
