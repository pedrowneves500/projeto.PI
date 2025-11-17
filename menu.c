#include "menu.h"
#include <raylib.h>
#include <math.h>

int selecionado = MENU_START_GAME1;
Texture2D menu_background = {0};
Font title_font = {0};
float creditosY = 680; // começa abaixo da tela (altura da tela)

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

    BeginDrawing();
        ClearBackground(BLACK);

        // Fundo com efeito ondulante
        DrawTextureRec(menu_background, src, (Vector2){ 0, 0 }, Fade(WHITE, 0.8f));

        // Filtro azul 
        DrawRectangle(0, 0, 1200, 680, Fade(BLUE, 0.15f));

        float brilho = (sin(tempo * 1.5f) + 1) / 2.0f; // valor entre 0 e 1
        DrawRectangle(0, 0, 1200, 680, Fade(SKYBLUE, brilho * 0.08f));

        // Textos do menu
        DrawTextEx(title_font, "Submerged Secrets 2", 
            (Vector2){ (1200 - MeasureText("Submerged Secrets 2", 60)) / 2 - 50, 100 }, 
            60, 2, GOLD);

        Color color_start1 = (selecionado == MENU_START_GAME1) ? ORANGE : BLACK;
        DrawText("FASE 1", (1200 - MeasureText("FASE 1", 30)) / 2, 250, 30, color_start1);

        Color color_start2 = (selecionado == MENU_START_GAME2) ? ORANGE : BLACK;
        DrawText("FASE 2", (1200 - MeasureText("FASE 2", 30)) / 2, 300, 30, color_start2);

        Color color_creditos = (selecionado == POS_CREDITOS) ? ORANGE : BLACK;
        DrawText("CREDITOS", (1200 - MeasureText("CREDITOS", 30)) / 2, 350, 30, color_creditos);

        Color color_exit = (selecionado == MENU_EXIT) ? RED : BLACK;
        DrawText("SAIR", (1200 - MeasureText("SAIR", 30)) / 2, 400, 30, color_exit);

    EndDrawing();

    return STATE_MENU;
}