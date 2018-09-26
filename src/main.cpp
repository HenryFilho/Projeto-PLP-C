#include <stdio.h>
#include "game.h"
#include "recordes.h"
#include "menu.h"
#include "creditos.h"

int main(){

	int gameOver = 0;
	while(1){
		int o = showMenu();

		switch(o)
		{	case 1:
				//OPCAO Iniciar
				gameOver = game();
				if (score > 0)
					verificaRecorde(score); //SÓ ENTRA SE SCORE >= 1
				break;
			case 2:
				//OPCAO Recordes
				imprimeRecordes();
				break;
			case 3:
				//OPCAO Creditos
				imprimeCreditos();
				break;
			case 4:
				//OPCAO Sair
				exit(0);
			default:
				break;
		}
	}

	return 0;
}
