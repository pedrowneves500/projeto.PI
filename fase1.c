#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include "fase1.h"

Celula labirinto[GRADE_COMP][GRADE_ALTURA];
Posicao jogador;


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
    //Declaração de variáveis
    labirinto[x][y].visitado = true;
    int direcoes[] = {0, 1, 2, 3}; //Norte, Leste, Sul, Oeste
    //Randomizando as direções a seguir
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
            GeradorLabirinto(nx,ny); //Chama recursiva
        }
    }

}



void DesenhoLabirinto() {
    for (int x = 0; x < GRADE_COMP; x++) {
        for( int y = 0; y < GRADE_ALTURA; y++) {
            int posicao_x = x * TAMANHO_CELULA;
            int posicao_y = y * TAMANHO_CELULA;

            if (labirinto[x][y].parede_norte) DrawLine(posicao_x, posicao_y,posicao_x + TAMANHO_CELULA, posicao_y,WHITE);
            if (labirinto[x][y].parede_leste) DrawLine(posicao_x + TAMANHO_CELULA, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA,WHITE);
            if (labirinto[x][y].parede_sul) DrawLine(posicao_x, posicao_y + TAMANHO_CELULA,posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA,WHITE);
            if (labirinto[x][y].parede_oeste) DrawLine(posicao_x, posicao_y,posicao_x, posicao_y + TAMANHO_CELULA,WHITE);

        }
    }
}

int fase1() {
    // Inicializa labirinto só uma vez
    static bool iniciado = false;
    if (!iniciado) {
        InicializaLabirinto();
        GeradorLabirinto(0,0);
        jogador.x = 0;
        jogador.y = 0;
        iniciado = true;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        return STATE_MENU;
    }
    // Desenho da fase
    BeginDrawing();
        ClearBackground(DARKGRAY);
        DesenhoLabirinto();
        // Desenhar jogador, itens, etc.
    EndDrawing();

    return STATE_FASE1;
}