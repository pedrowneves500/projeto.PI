#include "raylib.h"
#include <stdio.h>

static Texture2D menu_background = {0};

// Definição dos estados do jogo
typedef enum {
    STATE_MENU,
    STATE_FASE1,
    STATE_FASE2,
    STATE_SAIR
} GameState;

// Definição das opções do menu
typedef enum {
    MENU_START_GAME1 = 0,
    MENU_START_GAME2,
    MENU_EXIT,
    NUM_MENU_OPTIONS 
} MenuOptions;

// Variável para rastrear qual opção está selecionada
static int selecionado = MENU_START_GAME1; 

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
    }

    // Lógica de Desenho
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(menu_background, 0, 0, WHITE);

        DrawText("Submerged Secrets 2", 100, 50, 40, RAYWHITE);
        
        Color color_start1 = (selecionado == MENU_START_GAME1) ? YELLOW : RAYWHITE;
        DrawText("FASE 1", 150, 150, 30, color_start1);
        
        Color color_start2 = (selecionado == MENU_START_GAME2) ? YELLOW : RAYWHITE;
        DrawText("FASE 2", 150, 190, 30, color_start2);

        Color color_exit = (selecionado == MENU_EXIT) ? YELLOW : RAYWHITE;
        DrawText("SAIR", 150, 270, 30, color_exit);

    EndDrawing();

    return STATE_MENU;
}

int main(void) 
{
    // Configurações e Inicialização
    const int tela_comp = 626;
    const int tela_altura = 398;
    
    InitWindow(tela_comp, tela_altura, "Submerged Secrets 2");

    menu_background = LoadTexture("DESENHOS/background_menu.jpg");

    SetTargetFPS(60); 

    GameState estado_atual = STATE_MENU;
    
    // Loop Principal do Jogo
    while (!WindowShouldClose())    
    {
        switch (estado_atual)
        {
            case STATE_MENU:
            {
                estado_atual = run_menu(); // Roda o menu e pega o próximo estado
            } break;
            
            case STATE_FASE1:
            // Lógica da Fase 1
            {
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU;
                
                BeginDrawing();
                    ClearBackground(DARKGRAY);
                    DrawText("FASE 1", 100, 100, 30, GREEN);
                    DrawText("ESC para voltar ao Menu", 100, 150, 20, RAYWHITE);
                EndDrawing();
            } break;

            case STATE_FASE2:
            // Lógica da Fase 2
            {
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU;
                
                BeginDrawing();
                    ClearBackground(DARKGRAY);
                    DrawText("FASE 2", 100, 100, 30, BLUE);
                    DrawText("ESC para voltar ao Menu", 100, 150, 20, RAYWHITE);
                EndDrawing();
            } break;

            case STATE_SAIR:
            {
                // Isso fecha o loop e o programa
                goto exit_loop; 
            } break;
            
            default: break;
        }
    }

    exit_loop:
    UnloadTexture(menu_background); //Descarrega a tela
    CloseWindow();       // Fecha o programa.

    return 0;
}