#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <cstdlib>

#define VERTICAL 40
#define HORIZONTAL 200
#define DOWN 115
#define UP 119
#define LEFT 97
#define RIGHT 100

typedef struct Player {
	int height;
	int width;
	int yVelocity;
} Player;

typedef struct MovingPlatform {
    int X;
    int Y;
    char symbol;

} MovingPlatform;

void PlayerInit(Player *player);
void PlatformInit(Platform *platform);
void PlayerMove(Player *player, Velocity *vel);
void gotoxy(int,int);

int main() {

	Player player;

	return 0;
}
