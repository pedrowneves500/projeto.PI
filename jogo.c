#include "raylib.h"
#include <stdio.h>
#include <math.h>


static Texture2D menu_background = {0};
static Font title_font = {0};
static float creditosY = 680; // começa abaixo da tela (altura da tela)


// Definição dos estados do jogo
typedef enum {
    STATE_MENU,
    STATE_FASE1,
    STATE_FASE2,
    STATE_CREDITOS,
    STATE_SAIR
} GameState;

// Definição das opções do menu
typedef enum {
    MENU_START_GAME1 = 0,
    MENU_START_GAME2,
    POS_CREDITOS,
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

        if (selecionado == POS_CREDITOS) {
            return STATE_CREDITOS; 
        }
    }

    
    static float tempo = 0.0f;
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

        // Textos do men
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
