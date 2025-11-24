#ifndef FASE1_H
#define FASE1_H

#include "raylib.h"
#include <stdbool.h>

//CONSTANTES DO LABIRINTO
#define GRADE_COMP (TELA_COMP/TAMANHO_CELULA)
#define GRADE_ALTURA (TELA_ALTURA/TAMANHO_CELULA)
#define TAMANHO_CELULA 50
#define RAIO_LUZ 2 //raio de visão do jogador
#define MAX_VISAO 5
#define VELOCIDADE_JOGADOR 300.0f 
#define VELOCIDADE_TUBARAO_CONTINUO 200.0f
#define TELA_COMP 1500
#define TELA_ALTURA 850 
#define META_CRISTAIS 3
#define META_MOEDAS 5
#define META_CHAVES 3

// ESTRUTURAS E ENUMS
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
    float x;
    float y;
} PosicaoFloat;

typedef struct {
    int moedas;
    int cristais;
    int chaves;
} PlacarItens;

typedef struct {
    int moedas_necessarias;
    int cristais_necessarios;
    int chaves_necessarias;
} MetaColetaveis;

//VARIÁVEIS GLOBAIS EXTERNAS
extern Celula labirinto[GRADE_COMP][GRADE_ALTURA];
extern Posicao jogador;
extern Posicao Objetivo;
extern PlacarItens placar;
extern PosicaoFloat velocidadeDesenhoTubarao;
extern Texture2D fundo_fase1;
extern MetaColetaveis meta_vitoria;
extern Texture2D textura_jogador;
extern Texture2D textura_tubarao;
extern Texture2D textura_moeda;
extern Texture2D textura_cristal;
extern Texture2D textura_chave;

//PROTÓTIPOS DE FUNÇÃO
void InicializaLabirinto();
void GeradorLabirinto(int x, int y);
void AtualizaMovimento();
void TentaMoverJogador();
void DesenhoVisaoJogador();
void DesenhoJogador();
void DesenhoObjetivo(); 
int fase1();
bool VerificarColisao();
void ColocaItensAleatorios(); 
void ColetaItem();             
void DesenhaPlacar();          

#endif