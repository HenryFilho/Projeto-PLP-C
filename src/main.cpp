#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ncurses.h> // Biblioteca responsável pelo mvprintw
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctype.h>

#define VERTICAL 36
#define HORIZONTAL 50
#define DOWN 115
#define UP 119
#define LEFT 97
#define RIGHT 100
#define GRAVITY 3

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

//Game
void GameInit(Player *player);
void setBorders();
// Player
void PlayerInit(Player *player);
void PlatformInit(Platform *platform);
void drawPlayer(Player *player);
void erasePlayer(Player *player);
void movePlayer(Player *player);
void playerJump(Player *);
// Util
void debug(const char * msg);

int main() {
	char  cmd;
	initscr();
	keypad(stdscr, TRUE);
	noecho();
 	curs_set(FALSE); //Deixa o cursor invisível

	Player player;
	GameInit(&player);

	int z = 0;
	while(1) {
		cmd = toupper(getchar());
		switch (cmd) {
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

void GameInit (Player *player) {
	PlayerInit(player);
	setBorders();
	drawPlayer(player);
} 

void PlayerInit(Player *player) {
	player->height = 3;
	player->width = 3;
	player->x = (int) (HORIZONTAL/2) - 2;
	player->y = (int) VERTICAL - 3;
	player->MAX_SPEED = 85500;	
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
	int i = 0;

    for(i = 1; i <= GRAVITY; i++) {
        erasePlayer(player);
		player->y -= i;
		drawPlayer(player);
        usleep(player->MAX_SPEED);
	}
	for(i = 0; i <= GRAVITY; i++) {
		erasePlayer(player);
		player->y += i;
		drawPlayer(player);
         usleep(player->MAX_SPEED);
    }
}

void setBorders() {
	
    // int i;
	//for(i = 0; i < 20; i++) {
		for (int j = 0; j < HORIZONTAL + 1; j++) {
			if(j + 12 <= HORIZONTAL)
				mvprintw(0, j + 12, "X");
			mvprintw(VERTICAL, j, "X");
		}
		mvprintw(0, 1, " Pontuação:        ");
	//}
	//for(i = 0; i < 20; i++) {
		for (int j = 0; j < VERTICAL; j++) {
			mvprintw(j, 0, "&");
			mvprintw(j, HORIZONTAL, "&");
   		}
	//}

}

void debug(const char * msg) {
	fprintf(stderr, msg);
}

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