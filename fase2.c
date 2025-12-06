#include "fase2.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// --- ESTADOS DO TURNO ---
typedef enum {
    ETAPA_MIRAR,    
    ETAPA_CARREGAR, 
    ETAPA_DISPARO,  
    ETAPA_FIM       
} EtapaTurno;

// --- VARIÁVEIS ESTÁTICAS ---
static Submarino subs[2];
static ColunaAreia terreno[MAX_COLUNAS_TERRENO];
static Torpedo torpedo = { 0 };
static ExplosaoAgua explosao = { 0 };

static int jogadorDaVez = 0; 
static EtapaTurno etapaAtual = ETAPA_MIRAR;
static int direcaoOscilacao = 1; 
static int vencedor = 0;

// --- FUNÇÕES AUXILIARES ---

static void GerarTerreno() {
    float larguraColuna = (float)LARGURA_TELA_FASE2 / MAX_COLUNAS_TERRENO;
    int alturaAtual = ALTURA_TELA_FASE2 * 0.30; 
    
    for (int i = 0; i < MAX_COLUNAS_TERRENO; i++) {
        terreno[i].retangulo.x = i * larguraColuna;
        terreno[i].retangulo.width = larguraColuna + 1; 
        
        // Geração de Dunas
        int variacao = GetRandomValue(-10, 10); // Suavizado
        alturaAtual += variacao;

        // Limites
        if (alturaAtual < ALTURA_TELA_FASE2 * 0.15) alturaAtual = ALTURA_TELA_FASE2 * 0.15;
        if (alturaAtual > ALTURA_TELA_FASE2 * 0.55) alturaAtual = ALTURA_TELA_FASE2 * 0.55;

        terreno[i].retangulo.height = alturaAtual;
        terreno[i].retangulo.y = ALTURA_TELA_FASE2 - alturaAtual;
        
        terreno[i].cor = (Color){ 235, 200, 100, 255 }; 
    }
}

static void InicializarSubmarinos() {
    // Player 1
    subs[0].ativo = true;
    subs[0].isPlayer1 = true;
    subs[0].angulo = 0; 
    subs[0].forca = 0;
    subs[0].saude = 100;
    subs[0].cor = YELLOW;
    subs[0].temDisparoAnterior = false; // Começa sem memória
    
    int idx1 = GetRandomValue(15, 35);
    subs[0].posicao.x = terreno[idx1].retangulo.x + (terreno[idx1].retangulo.width / 2);
    subs[0].posicao.y = terreno[idx1].retangulo.y - 15;

    // Player 2
    subs[1].ativo = true;
    subs[1].isPlayer1 = false;
    subs[1].angulo = 180; 
    subs[1].forca = 0;
    subs[1].saude = 100;
    subs[1].cor = RED;
    subs[1].temDisparoAnterior = false; // Começa sem memória
    
    int idx2 = GetRandomValue(MAX_COLUNAS_TERRENO - 35, MAX_COLUNAS_TERRENO - 15);
    subs[1].posicao.x = terreno[idx2].retangulo.x + (terreno[idx2].retangulo.width / 2);
    subs[1].posicao.y = terreno[idx2].retangulo.y - 15;

    // Reset
    torpedo.ativo = false;
    explosao.ativo = false;
    etapaAtual = ETAPA_MIRAR;
    jogadorDaVez = 0;
    direcaoOscilacao = 1; 
    vencedor = 0;
}

static void DispararTorpedo() {
    // 1. Salva a memória do tiro atual antes de disparar
    subs[jogadorDaVez].ultimoAngulo = subs[jogadorDaVez].angulo;
    subs[jogadorDaVez].ultimaForca = subs[jogadorDaVez].forca;
    subs[jogadorDaVez].temDisparoAnterior = true;

    // 2. Configura o disparo
    float radianos = subs[jogadorDaVez].angulo * DEG2RAD;
    float offsetDisparo = 35.0f;
    torpedo.posicao.x = subs[jogadorDaVez].posicao.x + cos(radianos) * offsetDisparo;
    torpedo.posicao.y = subs[jogadorDaVez].posicao.y - sin(radianos) * offsetDisparo;

    torpedo.ativo = true;
    torpedo.raio = 5; 
    etapaAtual = ETAPA_DISPARO;

    float fatorPotencia = 0.20f; 
    torpedo.velocidade.x = cos(radianos) * subs[jogadorDaVez].forca * fatorPotencia;
    torpedo.velocidade.y = -sin(radianos) * subs[jogadorDaVez].forca * fatorPotencia; 
}

static void AtualizarLogica() {
    // --- 1. OSCILAÇÃO ---
    if (etapaAtual == ETAPA_MIRAR) {
        float velocidadeAngular = 1.0f;
        if (jogadorDaVez == 0) { 
            subs[0].angulo += (velocidadeAngular * direcaoOscilacao);
            if (subs[0].angulo >= 90) { subs[0].angulo = 90; direcaoOscilacao = -1; }
            if (subs[0].angulo <= 0)  { subs[0].angulo = 0;  direcaoOscilacao = 1; }
        } else { 
            subs[1].angulo -= (velocidadeAngular * direcaoOscilacao);
            if (subs[1].angulo <= 90)  { subs[1].angulo = 90;  direcaoOscilacao = -1; }
            if (subs[1].angulo >= 180) { subs[1].angulo = 180; direcaoOscilacao = 1; }
        }
    }
    else if (etapaAtual == ETAPA_CARREGAR) {
        float velocidadeForca = 0.8f; 
        subs[jogadorDaVez].forca += (velocidadeForca * direcaoOscilacao);
        if (subs[jogadorDaVez].forca >= 100) { subs[jogadorDaVez].forca = 100; direcaoOscilacao = -1; }
        if (subs[jogadorDaVez].forca <= 0)   { subs[jogadorDaVez].forca = 0;   direcaoOscilacao = 1; }
    }

    // --- 2. FÍSICA E COLISÃO ---
    if (etapaAtual == ETAPA_DISPARO && torpedo.ativo) {
        torpedo.posicao.x += torpedo.velocidade.x;
        torpedo.posicao.y += torpedo.velocidade.y;
        torpedo.velocidade.y += 0.15f; 

        bool colidiu = false;
        bool acertouChao = false;

        // A. Limites
        if (torpedo.posicao.x < 0 || torpedo.posicao.x > LARGURA_TELA_FASE2 || torpedo.posicao.y > ALTURA_TELA_FASE2) {
            colidiu = true;
        }

        // B. Colisão Terreno
        for (int i = 0; i < MAX_COLUNAS_TERRENO; i++) {
            if (CheckCollisionCircleRec(torpedo.posicao, torpedo.raio, terreno[i].retangulo)) {
                colidiu = true;
                acertouChao = true;
                break;
            }
        }

        // C. Colisão Submarinos
        for (int i = 0; i < 2; i++) {
            float dist = sqrt(pow(torpedo.posicao.x - subs[i].posicao.x, 2) + pow(torpedo.posicao.y - subs[i].posicao.y, 2));
            if (i == jogadorDaVez && dist < 50.0f) continue; 

            Rectangle recSub = { subs[i].posicao.x - 20, subs[i].posicao.y - 10, 40, 20 };
            if (CheckCollisionCircleRec(torpedo.posicao, torpedo.raio, recSub)) {
                colidiu = true;
                explosao.posicao = subs[i].posicao;
                explosao.ativo = true;
                explosao.raio = 60;
                explosao.tempoVida = 2.0f;
                subs[i].ativo = false;
                etapaAtual = ETAPA_FIM;
                vencedor = (i == 0) ? 2 : 1;
            }
        }

        if (colidiu && etapaAtual != ETAPA_FIM) {
            
            // --- TRATAMENTO DA DESTRUIÇÃO ---
            if (acertouChao) {
                // TRUQUE IMPORTANTE: Avança a explosão para DENTRO da areia
                // Multiplicamos a velocidade para projetar o ponto de impacto mais fundo
                explosao.posicao.x = torpedo.posicao.x + (torpedo.velocidade.x * 5.0f);
                explosao.posicao.y = torpedo.posicao.y + (torpedo.velocidade.y * 5.0f);
                
                explosao.ativo = true;
                explosao.raio = 70; // Cratera grande
                explosao.tempoVida = 1.0f;

                for (int i = 0; i < MAX_COLUNAS_TERRENO; i++) {
                    float centroColunaX = terreno[i].retangulo.x + (terreno[i].retangulo.width / 2);
                    float distanciaX = fabs(explosao.posicao.x - centroColunaX);

                    if (distanciaX < explosao.raio) {
                        float profundidade = sqrt(pow(explosao.raio, 2) - pow(distanciaX, 2));
                        
                        // Abaixa a areia
                        terreno[i].retangulo.height -= profundidade;
                        if (terreno[i].retangulo.height < 0) terreno[i].retangulo.height = 0;

                        // Atualiza visualmente
                        terreno[i].retangulo.y = ALTURA_TELA_FASE2 - terreno[i].retangulo.height;
                    }
                }
            }

            torpedo.ativo = false;
            jogadorDaVez = !jogadorDaVez;
            etapaAtual = ETAPA_MIRAR;
            direcaoOscilacao = 1;
            
            subs[jogadorDaVez].angulo = (jogadorDaVez == 0) ? 0 : 180;
            subs[jogadorDaVez].forca = 0;
        }
    }

    if (explosao.ativo) {
        explosao.tempoVida -= GetFrameTime();
        if (explosao.tempoVida <= 0) explosao.ativo = false;
    }
}

static void DesenharCena() {
    BeginDrawing();
    ClearBackground((Color){ 0, 40, 80, 255 }); 

    // 1. Terreno
    for (int i = 0; i < MAX_COLUNAS_TERRENO; i++) {
        DrawRectangleRec(terreno[i].retangulo, terreno[i].cor);
    }

    // 2. Submarinos
    for (int i = 0; i < 2; i++) {
        if (!subs[i].ativo) continue;

        DrawEllipse(subs[i].posicao.x, subs[i].posicao.y, 20, 10, subs[i].cor);
        DrawRectangle(subs[i].posicao.x - 5, subs[i].posicao.y - 15, 10, 10, subs[i].cor);

        // Se tiver tiro anterior, desenha a SETA FANTASMA (Transparente)
        if (subs[i].temDisparoAnterior) {
             float radGhost = subs[i].ultimoAngulo * DEG2RAD;
             Vector2 centro = subs[i].posicao;
             // Seta menor e transparente
             Vector2 pontaGhost = { centro.x + cos(radGhost) * 40, centro.y - sin(radGhost) * 40 };
             
             // Desenha linha fantasma
             DrawLineEx(centro, pontaGhost, 2, Fade(WHITE, 0.3f));
             
             // Opcional: Desenha barra fantasma
             /*
             if (i == jogadorDaVez && etapaAtual == ETAPA_CARREGAR) {
                 // Pode desenhar um marquinha na barra indicando a força anterior
                 // (Fica um pouco complexo visualmente, mantive só a seta por enquanto)
             }
             */
        }

        // Se for a vez do jogador: Seta Real e Barra
        if (i == jogadorDaVez && etapaAtual != ETAPA_DISPARO && etapaAtual != ETAPA_FIM) {
            float rad = subs[i].angulo * DEG2RAD;
            Vector2 centro = subs[i].posicao;
            Vector2 ponta = { centro.x + cos(rad) * 60, centro.y - sin(rad) * 60 };
            DrawLineEx(centro, ponta, 3, WHITE); // Seta Ativa (Sólida)
            
            if (etapaAtual == ETAPA_CARREGAR) {
                int xBarra = subs[i].posicao.x - 30;
                int yBarra = subs[i].posicao.y - 45; 
                DrawRectangle(xBarra, yBarra, 60, 8, BLACK); 
                DrawRectangle(xBarra, yBarra, (float)subs[i].forca/100 * 60, 8, ORANGE); 
                DrawRectangleLines(xBarra, yBarra, 60, 8, WHITE);
                
                // Marca da força anterior na barra atual (Opcional, mas útil)
                if (subs[i].temDisparoAnterior) {
                     float xMark = xBarra + ((float)subs[i].ultimaForca/100 * 60);
                     DrawLine(xMark, yBarra - 2, xMark, yBarra + 10, Fade(WHITE, 0.5f));
                }
            }
        }
    }

    // 3. Torpedo
    if (torpedo.ativo) {
        DrawCircleV(torpedo.posicao, torpedo.raio, BLACK);
    }

    // 4. Explosão
    if (explosao.ativo) {
        DrawCircleV(explosao.posicao, explosao.raio, Fade(ORANGE, 0.8f));
        DrawCircleV(explosao.posicao, explosao.raio * 0.7f, Fade(YELLOW, 0.8f));
    }

    // 5. HUD
    if (etapaAtual == ETAPA_FIM) {
        DrawText(TextFormat("JOGADOR %d VENCEU!", vencedor), LARGURA_TELA_FASE2/2 - 150, ALTURA_TELA_FASE2/2 - 20, 40, GREEN);
        DrawText("Pressione [ENTER] para Voltar", LARGURA_TELA_FASE2/2 - 160, ALTURA_TELA_FASE2/2 + 30, 20, WHITE);
    } else {
        const char* status = (etapaAtual == ETAPA_MIRAR) ? "MIRANDO..." : 
                             (etapaAtual == ETAPA_CARREGAR) ? "CARREGANDO FORCA..." : "TORPEDO NA AGUA!";
        Color corTexto = (jogadorDaVez == 0) ? YELLOW : RED;
        DrawText(TextFormat("VEZ DO JOGADOR %d: %s", jogadorDaVez + 1, status), 20, 20, 20, corTexto);
    }

    EndDrawing();
}

static void ProcessarEntrada() {
    bool acao = false;
    if (jogadorDaVez == 0 && IsKeyPressed(KEY_SPACE)) acao = true;
    if (jogadorDaVez == 1 && IsKeyPressed(KEY_ENTER)) acao = true;

    if (etapaAtual == ETAPA_MIRAR && acao) {
        etapaAtual = ETAPA_CARREGAR;
        direcaoOscilacao = 1;
    }
    else if (etapaAtual == ETAPA_CARREGAR && acao) {
        DispararTorpedo();
    }
}

int fase2() {
    GerarTerreno();
    InicializarSubmarinos();
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) return 0;

        if (etapaAtual == ETAPA_FIM) {
            if (IsKeyPressed(KEY_ENTER)) return vencedor;
        } else {
            ProcessarEntrada();
            AtualizarLogica();
        }

        DesenharCena();
    }
    return -1;
}