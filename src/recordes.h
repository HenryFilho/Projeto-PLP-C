#include <cstdlib>
#include <cstdio>
#include <cstring>

#define NUM_RECORDES 10

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
  printf("=========================\n");
  printf("======= RECORDES ========\n");
  printf("=========================\n\n");
  for (int i = 0; i < NUM_RECORDES; i++) {
    printf("%-20s %04d\n", recordes[i].nome, recordes[i].pontos);
  }
}
