#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ncurses.h>

#define NUM_RECORDES 10
#define SCR_WIDTH 150
#define SCR_HEIGHT 39
#define WIDTH 30
#define HEIGHT 12

int startx = 0;
int starty = 0;

char nome[21];

typedef struct Recorde {
  int pontos;
  char nome[21];
} Recorde;

Recorde recordes[NUM_RECORDES];

// Protótipos
void salvarNovoRecorde();
int verificaRecorde(int pontos);
void carregarRecordes();
void lerNome();
void inserirNovoRecorde(int pos, char nome[], int pontos);

int verificaRecorde(int pontos) {
  carregarRecordes();
  for (int i = 0; i < NUM_RECORDES; i++) {
    if (pontos > recordes[i].pontos) {
      strcpy(nome, "");
      lerNome();
      if (strcmp(nome, "") != 0) {
        inserirNovoRecorde(i, nome, pontos);
        salvarNovoRecorde();
      }
      break;
    }
  }
}

void carregarRecordes() {
  FILE *arquivoRecordes;
  arquivoRecordes = fopen("recordes.bin", "ab+");
  if (arquivoRecordes == NULL) {
    printf("Erro ao abrir arquivo de recordes! :(\n");
    exit(1);
  }
  rewind(arquivoRecordes);
  int i = 0;
  //printf("Bytes lidos: %lu\n", fread(&recordes[i], sizeof(Recorde), 1, arquivoRecordes));
  while(i < NUM_RECORDES && fread(&recordes[i], sizeof(Recorde), 1, arquivoRecordes) == 1) {
    //fprintf(stderr, "Entrei aqui\n");
    i++;
  }
  for (int j = i; j < NUM_RECORDES; j++) {
    strcpy(recordes[j].nome, "XXX");
    recordes[j].pontos = 0;
  }
  fclose(arquivoRecordes);
}

void lerNome() {
  printf("Digite seu nome: ");
  scanf("%[^\n]20s", nome);
}

void inserirNovoRecorde(int pos, char nome[], int pontos) {
  Recorde anterior = recordes[pos];
  Recorde aux;
  recordes[pos].pontos = pontos;
  strcpy(recordes[pos].nome, nome);
  for (int i = pos + 1; i < NUM_RECORDES; i++) {
    aux = recordes[i];
    recordes[i] = anterior;
    anterior = aux;
  }
}

void salvarNovoRecorde() {
  FILE *arquivoRecordes;
  arquivoRecordes = fopen("recordes.bin", "w+");
  if (arquivoRecordes == NULL) {
    printf("Erro ao abrir arquivo de recordes! :(\n");
    exit(1);
  }

  for (int j = 0; j < NUM_RECORDES; j++) {
    fwrite(&recordes[j], sizeof(Recorde), 1, arquivoRecordes);
  }
  fclose(arquivoRecordes);
}

void imprimeRecordes() {
    WINDOW *menu_win;
    int c;
    
    initscr();
    clear();
    noecho();
    curs_set(FALSE);
    cbreak();
    startx = (SCR_WIDTH - WIDTH) / 2;
    starty = (SCR_HEIGHT - HEIGHT) / 2;
    
    carregarRecordes();
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);

    int xName = (SCR_WIDTH - 25) / 2;
    mvprintw(starty - 3, xName, "=========================");
    mvprintw(starty - 2, xName, "======= RECORDES ========");
    mvprintw(starty - 1, xName, "=========================");

    refresh();
    int x, y, i;

    x = 3;
    y = 1;
    box(menu_win, 0, 0);
    for(i = 0; i < NUM_RECORDES; i++){
	    mvwprintw(menu_win, y, x, "%-20s %04d", recordes[i].nome, recordes[i].pontos);
	    y++;
    }
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
