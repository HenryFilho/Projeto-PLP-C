#include <stdio.h>
#include "game.h"
#include "recordes.h"

int main(){

	int algumacoisa;
	int gameOver = 0;
	while(1){
		scanf("%d", &algumacoisa);
		if(algumacoisa == 55 && !gameOver){
			gameOver = game();
			if (score > 0)
				verificaRecorde(score); //SÓ ENTRA SE SCORE >= 1
			imprimeRecordes();		
		}
	}

	return 0;
}