#include <raylib.h>
#include "menu.h"
#define TELA_COMP 1200
#define TELA_ALTURA 680
#define TAMANHO_CELULA 50
#define GRADE_COMP (TELA_COMP/TAMANHO_CELULA)
#define GRADE_ALTURA (TELA_ALTURA / TAMANHO_CELULA)
#define RAIO_LUZ 5

typedef struct {
    int visitado;
    int parede_norte, parede_sul, parede_oeste, parede_leste;
} Celula;

typedef struct {
    int x,y;
} Posicao;

extern Celula labirinto[GRADE_COMP][GRADE_ALTURA];
extern Posicao jogador;


void InicializaLabirinto(void);
int fase1(void);
void GeradorLabirinto(int x, int y);
void DesenhoLabirinto(void);


