#ifndef FASE2_H
#define FASE2_H

#include "raylib.h"
#include <stdbool.h>
#include <math.h>

// --- CONSTANTES ---
#define MAX_COLUNAS_TERRENO 150 
#define LARGURA_TELA_FASE2 1300
#define ALTURA_TELA_FASE2 650

// --- ESTRUTURAS ---
typedef struct {
    Vector2 posicao;
    bool ativo;
    bool isPlayer1; 
    
    // Dados Atuais
    float angulo;   
    float forca;    
    
    // Dados do Último Disparo (Memória)
    float ultimoAngulo;
    float ultimaForca;
    bool temDisparoAnterior; // Para saber se desenha a seta fantasma

    int saude;
    Color cor;
} Submarino;

typedef struct {
    Rectangle retangulo;
    Color cor;
} ColunaAreia;

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    int raio;
    bool ativo;
} Torpedo;

typedef struct {
    Vector2 posicao;
    int raio;
    bool ativo;
    float tempoVida;
} ExplosaoAgua;

int fase2(); 

#endif