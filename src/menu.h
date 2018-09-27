#ifndef MENU1_H_
#define MENU1_H_
#include <stdio.h>
#include <ncurses.h>
#define SCR_WIDTH 150 //AJUSTAR TAMANHO DA TELA AQUI !!!
#define SCR_HEIGHT 39
#define WIDTH 30
#define HEIGHT 10

//int startx = 0;
//int starty = 0;

char choices[][20] = {
			"Iniciar",
			"Recordes",
			"Creditos",
			"Sair"
		  };
	
//int n_choices = sizeof(choices) / sizeof(char *);
int n_choices = 4;
void print_menu(WINDOW *menu_win, int highlight);

int showMenu()
{	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
    curs_set(FALSE);
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (SCR_WIDTH - WIDTH) / 2;
	starty = (SCR_HEIGHT- HEIGHT) / 2;
		
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);

    int xName = (SCR_WIDTH - 26) / 2;
    mvprintw(starty - 3, xName, "==========================");
    mvprintw(starty - 2, xName, "======= JumpMaster =======");
    mvprintw(starty - 1, xName, "==========================");

	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}	
	clrtoeol();
	refresh();
	endwin();
	clear();
	return choice;
}


void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 11;
	y = 3;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

#endif
