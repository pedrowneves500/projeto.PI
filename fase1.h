#ifndef FASE1_H
#define FASE1_H

#include "raylib.h"
#include <stdbool.h>

// --- CONSTANTES DO LABIRINTO ---
#define GRADE_COMP 15
#define GRADE_ALTURA 10
#define TAMANHO_CELULA 40.0f
#define RAIO_LUZ 3 // Raio de visão do jogador
#define VELOCIDADE_JOGADOR 10.0f 

// --- CONSTANTES DE TELA (REVERTIDAS) ---
// A janela agora é do tamanho exato da grade, sem espaço lateral
#define SCREEN_WIDTH (GRADE_COMP * TAMANHO_CELULA) 
#define SCREEN_HEIGHT (GRADE_ALTURA * TAMANHO_CELULA) 

// Removemos #define LARGURA_PLACAR 250

// Estados do jogo
#define STATE_MENU 0
#define STATE_FASE1 1

// --- ESTRUTURAS E ENUMS ---

typedef enum {
    ITEM_NENHUM = 0,
    ITEM_MOEDA,      
    ITEM_CRISTAL,    
    ITEM_CHAVE       
} TipoItem;

typedef struct {
    bool visitado;
    bool parede_norte;
    bool parede_sul;
    bool parede_oeste;
    bool parede_leste;
    TipoItem item; 
} Celula; 

typedef struct {
    float x;      
    float y;
    int targetX;  
    int targetY;
} JogadorPosicao;

typedef struct {
    int x;
    int y;
} Posicao;

typedef struct {
    int moedas;
    int cristais;
    int chaves;
} PlacarItens;

// --- VARIÁVEIS GLOBAIS EXTERNAS ---
extern Celula labirinto[GRADE_COMP][GRADE_ALTURA];
extern JogadorPosicao jogador;
extern Posicao Objetivo;
extern PlacarItens placar;

// --- PROTÓTIPOS DE FUNÇÃO ---
void InicializaLabirinto();
void GeradorLabirinto(int x, int y);
void AtualizaMovimento();
void TentaMoverJogador();
void DesenhoVisaoJogador();
void DesenhoJogador();
void DesenhoObjetivo(); 
int fase1();

void ColocaItensAleatorios(); 
void ColetaItem();             
void DesenhaPlacar();          

#endif