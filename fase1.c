#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fase1.h"

Celula labirinto[GRADE_COMP][GRADE_ALTURA];
Posicao jogador;
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
void DesenhoVisaoJogador() {

    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) {
            
            int dist_x = abs(x - jogador.x);
            int dist_y = abs(y - jogador.y);
            

            if (dist_x <= RAIO_LUZ && dist_y <= RAIO_LUZ) {
                
                // Célula visível: desenha as paredes
                int posicao_x = x * TAMANHO_CELULA;
                int posicao_y = y * TAMANHO_CELULA;
                
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
    DrawRectangle(jogador.x  * TAMANHO_CELULA + 2, jogador.y * TAMANHO_CELULA +2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, WHITE);
}

void DesenhoObjetivo() {
    DrawRectangle(Objetivo.x *TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
}

void MoverJogador(int dx, int dy) {
    int novoX = jogador.x + dx;
    int novoY = jogador.y + dy;

    if (novoX < 0 || novoX >= GRADE_COMP || novoY < 0 || novoY >= GRADE_ALTURA) return;
    if (dx == -1 && labirinto[jogador.x][jogador.y].parede_oeste) return;
    if (dx == 1 && labirinto[jogador.x][jogador.y].parede_leste) return;
    if (dy == 1 && labirinto[jogador.x][jogador.y].parede_sul) return;
    if (dy == -1 && labirinto[jogador.x][jogador.y].parede_norte) return;

    jogador.x = novoX;
    jogador.y = novoY;
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
        Objetivo.x = GRADE_COMP -1;
        Objetivo.y = GRADE_ALTURA - 1;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        return STATE_MENU;
    }
    if (IsKeyPressed(KEY_RIGHT)) MoverJogador(1,0);
    if (IsKeyPressed(KEY_LEFT)) MoverJogador(-1,0);
    if (IsKeyPressed(KEY_UP)) MoverJogador(0,-1);
    if (IsKeyPressed(KEY_DOWN)) MoverJogador(0,1);

    // Desenho da fase
    BeginDrawing();
        ClearBackground(BLACK);
        DesenhoVisaoJogador();
        DesenhoJogador();
        DesenhoObjetivo();
    EndDrawing();

    return STATE_FASE1;
}