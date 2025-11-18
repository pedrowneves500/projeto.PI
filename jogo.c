#include "menu.h"
#include "fase1.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>
//gcc -o jogo jogo.c menu.c fase1.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm

int main(void) 
{
    // Configurações e Inicialização
    const int tela_comp = TELA_COMP;
    const int tela_altura = TELA_ALTURA;
    
    InitWindow(tela_comp, tela_altura, "Submerged Secrets 2");
    CarregarFundo1();
    SetExitKey(KEY_NULL); // O ESC não fecha mais a janela

    menu_background = LoadTexture("data/FUNDO_MENU.jpg");
    title_font = LoadFont("data/PERRYGOT.TTF");

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
            
            //Lógica da Fase 1
            case STATE_FASE1:
            {
                estado_atual = fase1();
            } break;

            // Lógica da Fase 2
            case STATE_FASE2:
            {
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU;
                
                BeginDrawing();
                    ClearBackground(DARKGRAY);
                    DrawText("FASE 2", 100, 100, 30, BLUE);
                    DrawText("ESC para voltar ao Menu", 100, 150, 20, RAYWHITE);
                EndDrawing();
            } break;

            //Lógica do Game Over
            case STATE_GAME_OVER:
            {
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU;
                BeginDrawing();
                    ClearBackground(DARKGRAY);
                    DrawText("GAME OVER", 100, 100, 30, BLUE);
                    DrawText("ESC para voltar ao Menu", 100, 150, 20, RAYWHITE);
                EndDrawing();

            } break;

            //Lógica dos Creditos
            case STATE_CREDITOS:
            {
                estado_atual = creditos();

            } break;
            //Logica do Sair

            case STATE_SAIR:
                goto exit_loop; // Sai do loop principal
                break;
                
            default: break;
        }
    }

    exit_loop:
    //Descarregamento de texturas
    UnloadTexture(menu_background); //Descarrega a tela
    UnloadTexture(fundo_fase1);
    CloseWindow();      // Fecha o programa.

    return 0;

}

