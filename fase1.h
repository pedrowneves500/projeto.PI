#include "raylib.h"
#include <stdio.h>
#include "menu.h"

//Jogo.c
#define TELA_COMP 1500
#define TELA_ALTURA 800

//fase1.c
#define TAMANHO_CELULA 50
#define GRADE_COMP (TELA_COMP/TAMANHO_CELULA)
#define GRADE_ALTURA (TELA_ALTURA / TAMANHO_CELULA)
#define RAIO_LUZ 2
#define MAX_VISAO 5
#define VELOCIDADE_JOGADOR (TAMANHO_CELULA * 8.0f)
#define VELOCIDADE_TUBARAO_CONTINUO 300.0f

typedef struct {
    int visitado;
    int parede_norte, parede_sul, parede_oeste, parede_leste;
} Celula;

typedef struct {
    int x,y;
} Posicao;

typedef struct PosicaoFloat {
    float x;
    float y;
} PosicaoFloat;

extern Celula labirinto[GRADE_COMP][GRADE_ALTURA];
extern Posicao jogador;
extern Posicao Objetivo;
extern PosicaoFloat posicaoDesenhoJogador;
extern Posicao tubarao;
extern PosicaoFloat posicaoDesenhoTubarao;
extern PosicaoFloat velocidadeDesenhoTubarao;

void CarregarFundo1();
void InicializaLabirinto(void);
int fase1(void);
void GeradorLabirinto(int x, int y);
void DesenhoLabirinto(void);
bool verificarColisao();

//Imagem de Fundo da Fase1
extern Texture2D fundo_fase1;