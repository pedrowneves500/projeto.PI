#include "menu.h"
#include "fase1.h"
#include "fase2.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>


extern float creditosY;
extern Texture2D menu_background;
extern Font title_font;
extern Texture2D fase1_fundo;

Sound som_coleta = {0};
Sound som_game_over = {0};
Sound som_game_win = {0};

// Comando de compilação para referência:
// gcc -o jogo jogo.c menu.c fase1.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm

int main(void) 
{
    // Configurações e Inicialização
    const int tela_comp = TELA_COMP;
    const int tela_altura = TELA_ALTURA;
    
    InitWindow(tela_comp, tela_altura, "Submerged Secrets 2");
    SetExitKey(KEY_NULL);

    // INICIALIZAÇÃO DE ÁUDIO E CARREGAMENTO
    InitAudioDevice(); 
    // O recurso de música é carregado no início.
    Music musica_menu = LoadMusicStream("audios/musica_menu.mp3");
    SetMusicVolume(musica_menu, 1.0f); 

    // Audios da fase 1
    Music musica_fase1 = LoadMusicStream("audios/musica_fase1.mp3");
    SetMusicVolume(musica_fase1, 0.8f);
    Sound som_coleta = LoadSound("audios/coletar.mp3");
    SetSoundVolume(som_coleta, 0.7f);
    Sound som_game_over = LoadSound("audios/gameover1.mp3");
    SetSoundVolume(som_game_over, 0.7f);
    Sound som_game_win = LoadSound("audios/gamewin1.mp3");
    SetSoundVolume(som_game_win, 0.7f);

    menu_background = LoadTexture("data/FUNDO_MENU.jpg");
    title_font = LoadFont("data/PERRYGOT.TTF"); 
    SetTargetFPS(60); 
    
    GameState estado_atual = STATE_MENU;
    
    // Loop Principal do Jogo
    while (!WindowShouldClose())
    {
        UpdateMusicStream(musica_menu);
        switch (estado_atual)
        {
            case STATE_MENU:
            {
                if (!IsMusicStreamPlaying(musica_menu)) {
                    PlayMusicStream(musica_menu);
                }

                GameState proximo_estado = run_menu(); 
                
                if (proximo_estado != STATE_MENU && proximo_estado != STATE_SAIR && proximo_estado != STATE_CREDITOS) {
                    StopMusicStream(musica_menu);
                }
                
                estado_atual = proximo_estado;

            } break;
            
            case STATE_FASE1:
            {
                if (!IsMusicStreamPlaying(musica_fase1)) {
                PlayMusicStream(musica_fase1);
                }
                
                UpdateMusicStream(musica_fase1); 

                GameState proximo_estado = fase1();

                if (proximo_estado != STATE_FASE1) {
                    StopMusicStream(musica_fase1);
                }

                estado_atual = proximo_estado;
            } break;

            case STATE_FASE2:
            {       fase2();
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU;
                
                BeginDrawing();
                    ClearBackground(DARKGRAY);
                    DrawText("FASE 2", 100, 100, 30, BLUE);
                    DrawText("ESC para voltar ao Menu", 100, 150, 20, RAYWHITE);
                EndDrawing();
            } break;

            case STATE_CREDITOS:
            {
                estado_atual = creditos();
            } break;
            
            case STATE_GAMEOVER:
            {
                estado_atual = gameover();
            } break;

            case STATE_SAIR:
                goto exit_loop;
                break;
            
            default: break;
        }
    }

    exit_loop:
    UnloadTexture(menu_background); 
    UnloadTexture(fundo_fase1);
    UnloadTexture(textura_jogador);
    UnloadTexture(textura_tubarao);
    UnloadTexture(textura_moeda);   
    UnloadTexture(textura_cristal);  
    UnloadTexture(textura_chave);
    UnloadMusicStream(musica_menu);
    UnloadMusicStream(musica_fase1); 
    UnloadSound(som_coleta);
    UnloadSound(som_game_over);
    UnloadSound(som_game_win);

    CloseAudioDevice(); 

    CloseWindow();

    return 0;

}