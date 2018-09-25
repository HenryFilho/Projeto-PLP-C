#include <stdio.h>
#include "game.h"

int main(){

	int algumacoisa;
	while(1){
		scanf("%d", &algumacoisa);
		if(algumacoisa == 55){
			game();
            fflush(stdin);
            while(getch())
			printf("AASD\n");
			verificaRecorde(score);
		}else if(algumacoisa == 0){
			break;
		}
	}

	return 0;
}