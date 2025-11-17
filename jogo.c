#include "menu.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>

int main(void) 
{
    // Configurações e Inicialização
    const int tela_comp = 1200;
    const int tela_altura = 680;
    
    InitWindow(tela_comp, tela_altura, "Submerged Secrets 2");
    SetExitKey(KEY_NULL); // O ESC não fecha mais a janela

    menu_background = LoadTexture("data/teste.jpg");
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

            case STATE_CREDITOS:
            {
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estado_atual = STATE_MENU;
                    creditosY = 680; // reinicia a posição da variável 
                }

                static float tempoCreditos = 0.0f;
                tempoCreditos += GetFrameTime();

                // Velocidade de subida dos créditos
                creditosY -= 40 * GetFrameTime(); 

                const int qtd_nomes = 5;
                const float espaco_entre = 50;
                float altura_total_creditos = qtd_nomes * espaco_entre;

                // reinicia a posição para começar de novo por baixo.
                if (creditosY + altura_total_creditos < 5)
                {
                    creditosY = 680; // Reseta para a posição inicial
                }

                // Fundo animado ondulante 
                float waveX = sin(tempoCreditos * 0.3f) * 3.0f;
                float waveY = cos(tempoCreditos * 0.2f) * 2.0f;
                Rectangle src = { waveX, waveY, menu_background.width, menu_background.height };

                BeginDrawing();
                    ClearBackground(BLACK);

                    // Fundo animado 
                    DrawTextureRec(menu_background, src, (Vector2){0, 0}, Fade(WHITE, 0.8f));
                    DrawRectangle(0, 0, 1200, 680, Fade(BLUE, 0.15f)); //filtro azul
                    float brilho = (sin(tempoCreditos * 1.5f) + 1) / 2.0f;
                    DrawRectangle(0, 0, 1200, 680, Fade(SKYBLUE, brilho * 0.08f));

                    // Pequenas bolhas subindo 
                    for(int i = 0; i < 20; i++){
                        float bolhaX = (i * 60 + fmod(tempoCreditos*20, 60)); 
                        float bolhaY = fmod(-i * 50 + tempoCreditos*30, 680); 
                        DrawCircle(bolhaX, 680 - bolhaY, 5, Fade(RAYWHITE, 0.15f));
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

                        DrawText(nomes[i],
                                 (1200 - MeasureText(nomes[i], 30 * escala))/2 + offsetX,
                                 y,
                                 30 * escala,
                                 Fade(RAYWHITE, alpha));
                    }

                EndDrawing();
            } break;
            case STATE_SAIR:
                goto exit_loop; // Sai do loop principal
                break;
                
            default: break;
        }
    }

    exit_loop:
    UnloadTexture(menu_background); //Descarrega a tela
    CloseWindow();      // Fecha o programa.

    return 0;

}

