#include "menu.h"
#include "fase1.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>

extern float creditosY;
extern Texture2D menu_background;
extern Font title_font;

// Comando de compilação para referência:
// gcc -o jogo jogo.c menu.c fase1.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm

int main(void) 
{
    // Configurações e Inicialização
    const int tela_comp = 1200;
    const int tela_altura = 680;
    
    InitWindow(tela_comp, tela_altura, "Submerged Secrets 2");
    SetExitKey(KEY_NULL);

    // INICIALIZAÇÃO DE ÁUDIO E CARREGAMENTO
    InitAudioDevice(); 
    
    // O recurso de música é carregado no início.
    Music musica_menu = LoadMusicStream("audios/musica_menu.mp3");
    SetMusicVolume(musica_menu, 1.0f); 

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
                if (!IsMusicStreamPlaying(musica_menu)) {
                    PlayMusicStream(musica_menu);
                }
                
                UpdateMusicStream(musica_menu);

                GameState proximo_estado = run_menu(); 
                
                if (proximo_estado != STATE_MENU && proximo_estado != STATE_SAIR) {
                    StopMusicStream(musica_menu);
                }
                
                estado_atual = proximo_estado;

            } break;
            
            case STATE_FASE1:
            {
                estado_atual = fase1();
            } break;

            case STATE_FASE2:
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
                        float alpha = 1.0f - 0.15f * fabs(dist);
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
                goto exit_loop;
                break;
                
            default: break;
        }
    }

    exit_loop:
    UnloadTexture(menu_background); 


    UnloadMusicStream(musica_menu); 
    CloseAudioDevice(); 


    CloseWindow();

    return 0;

}