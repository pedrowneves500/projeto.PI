#include "raylib.h"
#ifndef MENU_H
#define MENU_H
// Definição dos estados do jogo
typedef enum {
    STATE_MENU,
    STATE_FASE1,
    STATE_FASE2,
    STATE_CREDITOS,
    STATE_SAIR,
    STATE_GAMEOVER
} GameState;

// Definição das opções do menu
typedef enum {
    MENU_START_GAME1 = 0,
    MENU_START_GAME2,
    POS_CREDITOS,
    MENU_EXIT,
    NUM_MENU_OPTIONS
} MenuOptions;

GameState run_menu(void);
GameState creditos();
GameState gameover();
extern Texture2D menu_background;
extern Font title_font;
extern float creditosY;
extern Texture2D fundo_fase1;
#endif