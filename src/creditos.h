void imprimeCreditos() {
    WINDOW *menu_win;
    int c;
    
    initscr();
    clear();
    noecho();
    curs_set(FALSE);
    cbreak();
    startx = (SCR_WIDTH - WIDTH) / 2;
    starty = (SCR_HEIGHT - HEIGHT) / 2;
    
   
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);

    int xName = (SCR_WIDTH - 25) / 2;
    mvprintw(starty - 3, xName, "*************************");
    mvprintw(starty - 2, xName, "******* Creditos ********");
    mvprintw(starty - 1, xName, "*************************");

    refresh();
    int x, y, i;

    x = 3;
    y = 1;
    box(menu_win, 0, 0);
    
	mvwprintw(menu_win, y, x, "Rafael Pontes");
	y++;
	mvwprintw(menu_win, y, x, "Henry Filho");
	y++;
	mvwprintw(menu_win, y, x, "Junior Mendes");
	y++;
	mvwprintw(menu_win, y, x, "Matheus Santana");
	y++;
    mvwprintw(menu_win, y, x, "Jeronimo Jairo");
    
    while(1){
	    c = wgetch(menu_win);
	    if(c == 10)
		    break;
    }
    clrtoeol();
    refresh();
    endwin();
    clear();
}
