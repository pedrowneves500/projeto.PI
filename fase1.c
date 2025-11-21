#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h> 
#include "fase1.h"

Celula labirinto[GRADE_COMP][GRADE_ALTURA];
JogadorPosicao jogador; 
Posicao Objetivo;

void InicializaLabirinto() {
    for(int i = 0;i < GRADE_COMP; i++) {
         for(int j = 0; j < GRADE_ALTURA; j++) {
          labirinto[i][j].visitado = false;
            labirinto[i][j].parede_norte = true;
            labirinto[i][j].parede_sul = true;
            labirinto[i][j].parede_oeste = true;
            labirinto[i][j].parede_leste = true;
        }
    }
}

void GeradorLabirinto(int x, int y) {

    labirinto[x][y].visitado = true;
    int direcoes[] = {0, 1, 2, 3}; //Norte, Leste, Sul, Oeste

    for (int i = 0; i < 4 ; i++) {
       int j = GetRandomValue(i, 3);
        int temp = direcoes[i];
        direcoes[i] = direcoes[j];
        direcoes[j] = temp;
    }
    for (int i = 0; i < 4; i++) {
        int nx = x; 
        int ny = y;
        if (direcoes[i] == 0) {
            ny = y - 1;    //Norte
        } else if (direcoes[i] == 1) {
            nx = x + 1;    //Leste
        } else if (direcoes[i] == 2) {
            ny = y + 1;    //Sul
        } else if (direcoes[i] == 3) {
            nx = x - 1;    //Oeste     
        }
        if (nx >= 0 && nx < GRADE_COMP && ny >= 0 && ny < GRADE_ALTURA  && !labirinto[nx][ny].visitado) {
            if (direcoes[i] == 0 ) {
                labirinto[x][y].parede_norte = false;
                labirinto[nx][ny].parede_sul = false;
            } else if (direcoes[i] == 1) {
                labirinto[x][y].parede_leste = false;
                labirinto[nx][ny].parede_oeste = false;
            } else if (direcoes[i] == 2) {
                labirinto[x][y].parede_sul = false;
                labirinto[nx][ny].parede_norte = false;
            } else if (direcoes[i] == 3) {
                labirinto[x][y].parede_oeste = false;
                labirinto[nx][ny].parede_leste = false;
            }
            GeradorLabirinto(nx,ny); 
        }
    }
}

void AtualizaMovimento() {
    float frameTime = GetFrameTime();
    
    // Interpolação no eixo X
    float destinoX = (float)jogador.targetX;
    if (fabs(destinoX - jogador.x) > 0.001f) {
        float moveX = VELOCIDADE_JOGADOR * frameTime;
        if (destinoX > jogador.x) {
            jogador.x += moveX;
            if (jogador.x > destinoX) jogador.x = destinoX;
        } else if (destinoX < jogador.x) {
            jogador.x -= moveX;
            if (jogador.x < destinoX) jogador.x = destinoX;
        }
    } else {
        jogador.x = destinoX; 
    }
    
    // Interpolação no eixo Y
    float destinoY = (float)jogador.targetY;
    if (fabs(destinoY - jogador.y) > 0.001f) {
        float moveY = VELOCIDADE_JOGADOR * frameTime;
        if (destinoY > jogador.y) {
            jogador.y += moveY;
            if (jogador.y > destinoY) jogador.y = destinoY;
        } else if (destinoY < jogador.y) {
            jogador.y -= moveY;
            if (jogador.y < destinoY) jogador.y = destinoY;
        }
    } else {
        jogador.y = destinoY; // Garante a posição final exata
    }
}

void TentaMoverJogador() {
    // Só permite nova entrada se o jogador não estiver em movimento
    if (fabs(jogador.x - jogador.targetX) > 0.001f || fabs(jogador.y - jogador.targetY) > 0.001f) return;

    float dx = 0;
    float dy = 0;

    // Prioriza input horizontal, para evitar movimento diagonal
    if (IsKeyDown(KEY_RIGHT)) dx = 1;
    else if (IsKeyDown(KEY_LEFT)) dx = -1;

    // Se não houver movimento horizontal, tenta o vertical
    if (dx == 0) {
        if (IsKeyDown(KEY_DOWN)) dy = 1;
        else if (IsKeyDown(KEY_UP)) dy = -1;
    }
    
    // Se nenhum input válido for detectado, saia.
    if (dx == 0 && dy == 0) return;

    float novoX = jogador.targetX + dx;
    float novoY = jogador.targetY + dy;

    // 1. Verificação de Limites da Grade
    if (novoX < 0 || novoX >= GRADE_COMP || novoY < 0 || novoY >= GRADE_ALTURA) return;

    // 2. Verificação de Colisão com Paredes
    int currentX = jogador.targetX;
    int currentY = jogador.targetY;
    
    if (dx == -1 && labirinto[currentX][currentY].parede_oeste) return;
    if (dx == 1 && labirinto[currentX][currentY].parede_leste) return;
    if (dy == 1 && labirinto[currentX][currentY].parede_sul) return;
    if (dy == -1 && labirinto[currentX][currentY].parede_norte) return;

    jogador.targetX = novoX;
    jogador.targetY = novoY;
}


void DesenhoVisaoJogador() {

    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) { 

            float dist_x = fabs(x - jogador.targetX); 
            float dist_y = fabs(y - jogador.targetY); 

            if (dist_x <= RAIO_LUZ && dist_y <= RAIO_LUZ) {
 
                float posicao_x = x * TAMANHO_CELULA;
                float posicao_y = y * TAMANHO_CELULA;
 
                if (labirinto[x][y].parede_norte) DrawLine(posicao_x, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y, WHITE);
                if (labirinto[x][y].parede_leste) DrawLine(posicao_x + TAMANHO_CELULA, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_sul) DrawLine(posicao_x, posicao_y + TAMANHO_CELULA, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_oeste) DrawLine(posicao_x, posicao_y, posicao_x, posicao_y + TAMANHO_CELULA, WHITE);
                if (x == Objetivo.x && y == Objetivo.y) {
                  DrawRectangle(Objetivo.x * TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
                }
            }
         }
    } 
}

void DesenhoJogador() {
    DrawRectangle(
        (int)(jogador.x * TAMANHO_CELULA) + 2,  
        (int)(jogador.y * TAMANHO_CELULA) + 2,  
        TAMANHO_CELULA - 4, 
        TAMANHO_CELULA - 4, 
        GREEN
    );
}

void DesenhoObjetivo() {
    DrawRectangle(Objetivo.x *TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
}

int fase1() {
    static bool iniciado = false;
    if (!iniciado) {
        InicializaLabirinto();
        GeradorLabirinto(0,0);
        jogador.x = 0.0f;
        jogador.y = 0.0f;
        jogador.targetX = 0;
        jogador.targetY = 0;
 
        iniciado = true;
        Objetivo.x = GRADE_COMP -1;
        Objetivo.y = GRADE_ALTURA - 1;
    } 
    // --- LÓGICA DE JOGO ---
    
    TentaMoverJogador();

    AtualizaMovimento();

    // Input de menu/saída
    if (IsKeyPressed(KEY_ESCAPE)) {
        return STATE_MENU;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DesenhoVisaoJogador();
    DesenhoJogador();
    DesenhoObjetivo();
    EndDrawing();
    return STATE_FASE1;
}