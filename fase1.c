#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h> 
#include "fase1.h" // Inclui o cabeçalho com as novas estruturas e constantes de tela

// --- VARIÁVEIS GLOBAIS ---
Celula labirinto[GRADE_COMP][GRADE_ALTURA];
JogadorPosicao jogador; 
Posicao Objetivo;
PlacarItens placar; 

// --- FUNÇÕES DE INICIALIZAÇÃO E GERAÇÃO ---

void InicializaLabirinto() {
    for(int i = 0; i < GRADE_COMP; i++) {
        for(int j = 0; j < GRADE_ALTURA; j++) {
            labirinto[i][j].visitado = false;
            labirinto[i][j].parede_norte = true;
            labirinto[i][j].parede_sul = true;
            labirinto[i][j].parede_oeste = true;
            labirinto[i][j].parede_leste = true;
            labirinto[i][j].item = ITEM_NENHUM; 
        }
    }
}

void GeradorLabirinto(int x, int y) {
    labirinto[x][y].visitado = true;
    int direcoes[] = {0, 1, 2, 3}; 

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
            ny = y - 1; 
        } else if (direcoes[i] == 1) {
            nx = x + 1; 
        } else if (direcoes[i] == 2) {
            ny = y + 1; 
        } else if (direcoes[i] == 3) {
            nx = x - 1; 
        }
        if (nx >= 0 && nx < GRADE_COMP && ny >= 0 && ny < GRADE_ALTURA && !labirinto[nx][ny].visitado) {
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

void ColocaItensAleatorios() {
    const int NUM_MOEDAS = 15;
    const int NUM_CRISTAIS = 8;
    const int NUM_CHAVES = 3; 
    
    int totalItens = NUM_MOEDAS + NUM_CRISTAIS + NUM_CHAVES;
    
    if (totalItens >= GRADE_COMP * GRADE_ALTURA - 2) {
        totalItens = GRADE_COMP * GRADE_ALTURA - 3; 
    }

    int itensColocados = 0;

    while (itensColocados < totalItens) {
        int x = GetRandomValue(0, GRADE_COMP - 1);
        int y = GetRandomValue(0, GRADE_ALTURA - 1);

        if ((x == 0 && y == 0) || (x == Objetivo.x && y == Objetivo.y)) continue;
        
        if (labirinto[x][y].item == ITEM_NENHUM) {
            TipoItem novoItem = ITEM_NENHUM;
            
            if (itensColocados < NUM_MOEDAS) {
                novoItem = ITEM_MOEDA;
            } else if (itensColocados < NUM_MOEDAS + NUM_CRISTAIS) {
                novoItem = ITEM_CRISTAL;
            } else {
                novoItem = ITEM_CHAVE;
            }
            
            labirinto[x][y].item = novoItem;
            itensColocados++;
        }
    }
}

// --- FUNÇÕES DE MOVIMENTO E COLETA (SEM ALTERAÇÕES) ---
void AtualizaMovimento() {
    float frameTime = GetFrameTime();
    
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
        jogador.y = destinoY;
    }
}

void TentaMoverJogador() {
    if (fabs(jogador.x - jogador.targetX) > 0.001f || fabs(jogador.y - jogador.targetY) > 0.001f) return;

    float dx = 0;
    float dy = 0;

    if (IsKeyDown(KEY_RIGHT)) dx = 1;
    else if (IsKeyDown(KEY_LEFT)) dx = -1;

    if (dx == 0) {
        if (IsKeyDown(KEY_DOWN)) dy = 1;
        else if (IsKeyDown(KEY_UP)) dy = -1;
    }
    
    if (dx == 0 && dy == 0) return;

    float novoX = jogador.targetX + dx;
    float novoY = jogador.targetY + dy;

    if (novoX < 0 || novoX >= GRADE_COMP || novoY < 0 || novoY >= GRADE_ALTURA) return;

    int currentX = jogador.targetX;
    int currentY = jogador.targetY;
    
    if (dx == -1 && labirinto[currentX][currentY].parede_oeste) return;
    if (dx == 1 && labirinto[currentX][currentY].parede_leste) return;
    if (dy == 1 && labirinto[currentX][currentY].parede_sul) return;
    if (dy == -1 && labirinto[currentX][currentY].parede_norte) return;

    jogador.targetX = novoX;
    jogador.targetY = novoY;
}

void ColetaItem() {
    int x = jogador.targetX;
    int y = jogador.targetY;
    TipoItem itemColetado = labirinto[x][y].item;

    if (itemColetado != ITEM_NENHUM) {
        switch (itemColetado) {
            case ITEM_MOEDA:
                placar.moedas++;
                break;
            case ITEM_CRISTAL:
                placar.cristais++;
                break;
            case ITEM_CHAVE:
                placar.chaves++;
                break;
            default:
                break;
        }
        labirinto[x][y].item = ITEM_NENHUM; 
    }
}

// --- FUNÇÕES DE DESENHO ---

void DesenhoVisaoJogador() {
    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) { 

            float dist_x = fabs((float)x - jogador.targetX); 
            float dist_y = fabs((float)y - jogador.targetY); 

            // Se você usou o teste "if (true)" para ver todos os itens, 
            // volte para esta condição:
            if (dist_x <= RAIO_LUZ && dist_y <= RAIO_LUZ) {
            
                float posicao_x = x * TAMANHO_CELULA;
                float posicao_y = y * TAMANHO_CELULA;

                if (labirinto[x][y].item != ITEM_NENHUM) {
                    Color corItem = GRAY;
                    float raioItem = TAMANHO_CELULA / 3.0f;
                    switch (labirinto[x][y].item) {
                        case ITEM_MOEDA: 
                            corItem = YELLOW; 
                            break;
                        case ITEM_CRISTAL: 
                            corItem = VIOLET; 
                            raioItem = TAMANHO_CELULA / 4.0f;
                            break;
                        case ITEM_CHAVE: 
                            corItem = GOLD; 
                            break;
                        default:
                            break;
                    }
                    DrawCircle(posicao_x + TAMANHO_CELULA/2, posicao_y + TAMANHO_CELULA/2, raioItem, corItem);
                }
                
                if (x == Objetivo.x && y == Objetivo.y) {
                    DrawRectangle(Objetivo.x * TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
                }
                
                if (labirinto[x][y].parede_norte) DrawLine(posicao_x, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y, WHITE);
                if (labirinto[x][y].parede_leste) DrawLine(posicao_x + TAMANHO_CELULA, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_sul) DrawLine(posicao_x, posicao_y + TAMANHO_CELULA, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_oeste) DrawLine(posicao_x, posicao_y, posicao_x, posicao_y + TAMANHO_CELULA, WHITE);
            }
        }
    } 
}

void DesenhoJogador() {
    DrawRectangle(
        (int)(jogador.x * TAMANHO_CELULA) + 2, 
        (int)(jogador.y * TAMANHO_CELULA) + 2, 
        (int)TAMANHO_CELULA - 4, 
        (int)TAMANHO_CELULA - 4, 
        GREEN
    );
}

void DesenhoObjetivo() {} // Mantida vazia
void DesenhaPlacar() {
    // Largura que o placar ocupará
    const int LARGURA_PLACAR = 240; 
    
    int startY = 10;
    int fontSize = 20;
    
    // Pega a largura da tela ATUAL (que agora é só o labirinto)
    int screenWidth = GetScreenWidth(); 
    
    // Posição X de início: Largura da tela - Largura do placar
    int inicioX = screenWidth - LARGURA_PLACAR;

    // Fundo do placar
    DrawRectangle(inicioX, 0, LARGURA_PLACAR, 100, Fade(DARKGRAY, 0.7f)); // Altura 100 pixels
    
    // Posição X do texto (10px de margem interna)
    int textoX = inicioX + 10;
    
    // Moedas
    DrawText(TextFormat("MOEDAS: %d", placar.moedas), textoX, startY, fontSize, YELLOW);
    
    // Cristais
    DrawText(TextFormat("CRISTAIS: %d", placar.cristais), textoX, startY + fontSize + 5, fontSize, VIOLET);
    
    // Chaves
    DrawText(TextFormat("CHAVES: %d", placar.chaves), textoX, startY + 2 * (fontSize + 5), fontSize, GOLD);
}

// --- FUNÇÃO PRINCIPAL DA FASE ---

int fase1() {
    static bool iniciado = false;
    
    // 1. Inicializa a Janela com as dimensões da GRADE
    if (!IsWindowReady()) {
        // Usa SCREEN_WIDTH/HEIGHT definidas em fase1.h (Grade * Célula)
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Seu Labirinto com Placar");
        SetTargetFPS(60); 
    }    
    // 2. Inicialização da Lógica do Jogo
    if (!iniciado) {
        InicializaLabirinto();
        GeradorLabirinto(0,0);
        
        // Inicializa Jogador e Objetivo
        jogador.x = 0.0f;
        jogador.y = 0.0f;
        jogador.targetX = 0;
        jogador.targetY = 0;
        Objetivo.x = GRADE_COMP -1;
        Objetivo.y = GRADE_ALTURA - 1;
        
        // Inicializa Placar e Gera Itens
        placar.moedas = 0;
        placar.cristais = 0;
        placar.chaves = 0;
        ColocaItensAleatorios();
        
        iniciado = true;
    } 
    
    // --- LÓGICA DE JOGO ---
    
    TentaMoverJogador();

    if (fabs(jogador.x - jogador.targetX) < 0.001f && fabs(jogador.y - jogador.targetY) < 0.001f) {
        ColetaItem();
    }
    
    AtualizaMovimento();

    // Input de menu/saída
    if (IsKeyPressed(KEY_ESCAPE)) {
        // Assume que você quer fechar a janela ao sair da fase
        if (IsWindowReady()) CloseWindow(); 
        return STATE_MENU;
    }

    // Verifica condição de vitória
    if (jogador.targetX == Objetivo.x && jogador.targetY == Objetivo.y) {
        iniciado = false;
        return STATE_MENU; 
    }

    // --- DESENHO ---
    
    BeginDrawing();
    ClearBackground(BLACK);
    
    DesenhoVisaoJogador(); 
    DesenhoJogador();
    DesenhaPlacar();       
    
    EndDrawing();
    
    return STATE_FASE1;
}