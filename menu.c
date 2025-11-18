#include "menu.h"
#include "fase1.h"
#include "raylib.h"
#include <math.h>

int selecionado = MENU_START_GAME1;
Texture2D menu_background = {0};
Font title_font = {0};
float creditosY = TELA_ALTURA; // começa abaixo da tela (altura da tela)

GameState run_menu() {
    
    if (IsKeyPressed(KEY_DOWN)) {
        selecionado = (selecionado + 1) % NUM_MENU_OPTIONS;
    }
    if (IsKeyPressed(KEY_UP)) {
        selecionado = (selecionado - 1 + NUM_MENU_OPTIONS) % NUM_MENU_OPTIONS;
    }

    // Confirmar a seleção com ENTER
    if (IsKeyPressed(KEY_ENTER)) {
        if (selecionado == MENU_START_GAME1) {
            return STATE_FASE1; 
        }

        if (selecionado == MENU_START_GAME2){
            return STATE_FASE2; 
        }

        if (selecionado == MENU_EXIT) {
            return STATE_SAIR; 
        }

        if (selecionado == POS_CREDITOS) {
            return STATE_CREDITOS; 
        }
    }

    
    float tempo = 0.0f;
    tempo += GetFrameTime(); // atualiza tempo contínuo

    // Movimento ondulante 
    float waveOffsetX = sin(tempo * 0.3f) * 3.0f;  // eixo X
    float waveOffsetY = cos(tempo * 0.2f) * 2.0f;  // eixo Y

    Rectangle src = { waveOffsetX, waveOffsetY, menu_background.width, menu_background.height };

    Rectangle dest = { 0.0f, 0.0f, (float)TELA_COMP, (float)TELA_ALTURA };

    BeginDrawing();
        ClearBackground(BLACK); 

        // Fundo com efeito ondulante
        DrawTexturePro(menu_background, src, dest, (Vector2){ 0, 0 }, 0.0f, Fade(WHITE, 0.8f));

        // Filtro azul 
        DrawRectangle(0, 0, TELA_COMP, TELA_ALTURA, Fade(BLUE, 0.15f));

        float brilho = (sin(tempo * 1.5f) + 1) / 2.0f; // valor entre 0 e 1
        DrawRectangle(0, 0, TELA_COMP, TELA_ALTURA, Fade(SKYBLUE, brilho * 0.08f));

        // Textos do menu
        DrawTextEx(title_font, "Submerged Secrets 2", 
            (Vector2){ (TELA_COMP - MeasureText("Submerged Secrets 2", 60)) / 2 - 50, 100 }, 
            60, 2, GOLD);

        Color color_start1 = (selecionado == MENU_START_GAME1) ? ORANGE : BLACK;
        DrawText("FASE 1", (TELA_COMP - MeasureText("FASE 1", 30)) / 2, 250, 30, color_start1);

        Color color_start2 = (selecionado == MENU_START_GAME2) ? ORANGE : BLACK;
        DrawText("FASE 2", (TELA_COMP - MeasureText("FASE 2", 30)) / 2, 300, 30, color_start2);

        Color color_creditos = (selecionado == POS_CREDITOS) ? ORANGE : BLACK;
        DrawText("CREDITOS", (TELA_COMP - MeasureText("CREDITOS", 30)) / 2, 350, 30, color_creditos);

        Color color_exit = (selecionado == MENU_EXIT) ? RED : BLACK;
        DrawText("SAIR", (TELA_COMP - MeasureText("SAIR", 30)) / 2, 400, 30, color_exit);

    EndDrawing();

    return STATE_MENU;
}

int creditos() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        return STATE_MENU;
        creditosY = TELA_ALTURA; // reinicia a posição da variável 
    }

    static float tempoCreditos = 0.0f;
    tempoCreditos += GetFrameTime();

    // Velocidade de subida dos créditos
    creditosY -= 40 * GetFrameTime(); 

    const int qtd_nomes = 5;
    const float espaco_entre = 50;
    float altura_total_creditos = qtd_nomes * espaco_entre;

    // reinicia a posição para começar de novo por baixo.
    if (creditosY + altura_total_creditos < 5){
        creditosY = TELA_ALTURA; // Reseta para a posição inicial
    }

    // Fundo animado ondulante 
    float waveX = sin(tempoCreditos * 0.3f) * 3.0f;
    float waveY = cos(tempoCreditos * 0.2f) * 2.0f;
    Rectangle src = { waveX, waveY, menu_background.width, menu_background.height };
    Rectangle dest = { 0.0f, 0.0f, (float)TELA_COMP, (float)TELA_ALTURA };
    
    BeginDrawing();
    ClearBackground(BLACK);

    // Fundo animado
    DrawTexturePro(menu_background, src, dest, (Vector2){ 0, 0 }, 0.0f, Fade(WHITE, 0.8f));
    float brilho = (sin(tempoCreditos * 1.5f) + 1) / 2.0f;
    DrawRectangle(0, 0, TELA_COMP, TELA_ALTURA, Fade(SKYBLUE, brilho * 0.08f));

    // Pequenas bolhas subindo 
    for(int i = 0; i < 20; i++){
        float bolhaX = (i * 60 + fmod(tempoCreditos*20, 60)); 
        float bolhaY = fmod(-i * 50 + tempoCreditos*30, TELA_ALTURA); 
        DrawCircle(bolhaX, TELA_ALTURA - bolhaY, 5, Fade(RAYWHITE, 0.15f));
    }

    // Título dos créditos 
    DrawText("CREDITOS", 100, 50, 40, GOLD);

    // Lista de nomes 
    const char* nomes[] = {
    "Daniel Naslavsky",
    "Joelle Calado",
    "Marcela Massa",
    "Paulo Braz",
    "Pedro Henrique Neves"
    };

    for(int i = 0; i < qtd_nomes; i++){
        float y = creditosY + i * espaco_entre;

        // Efeitos visuais dos nomes 
        float dist = (y - 340)/340.0f;
        float escala = 1.0f - 0.05f * fabs(dist);
        float alpha  = 1.0f - 0.15f * fabs(dist);
        float offsetX = sin(tempoCreditos * 0.5f + i) * 40;

        DrawText(nomes[i],(TELA_COMP - MeasureText(nomes[i], 30 * escala))/2 + offsetX, y, 30 * escala, Fade(RAYWHITE, alpha));
    }
    EndDrawing();
    return STATE_CREDITOS;
}