#include "raylib.h"
#include <stdio.h>  
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "fase1.h"
#include "menu.h"

// --- VARIÁVEIS GLOBAIS ---
Celula labirinto[GRADE_COMP][GRADE_ALTURA];
Posicao jogador;
Posicao Objetivo;
PosicaoFloat posicaoDesenhoJogador;
Posicao tubarao;
PosicaoFloat posicaoDesenhoTubarao;
PosicaoFloat velocidadeDesenhoTubarao = {0};
Texture2D fundo_fase1 = {0};
PlacarItens placar;
MetaColetaveis meta_vitoria;
Texture2D textura_jogador = {0};
Texture2D textura_tubarao = {0};
Texture2D textura_moeda = {0};
Texture2D textura_cristal = {0};
Texture2D textura_chave = {0};

void CarregarRecursosFase1() {
    fundo_fase1 = LoadTexture("data/fundo_fase1.jpg");

    textura_jogador = LoadTexture("data/mergulhador_fase1.png"); 
    textura_tubarao = LoadTexture("data/tubarao_fase1.png");
    textura_moeda = LoadTexture("data/moeda_fase1.png");
    textura_cristal = LoadTexture("data/cristal_fase1.png");
    textura_chave = LoadTexture("data/chave_fase1.png");
}

bool VerificarColisao() {
    // Colisão ocorre se o jogador e o tubarão ocuparem a mesma posição
    if (jogador.x == tubarao.x && jogador.y == tubarao.y) {
        return true;
    }
    return false;
}
void InicializaLabirinto() {
    for(int i = 0;i < GRADE_COMP; i++) {
        for(int j = 0; j < GRADE_ALTURA; j++) {
            labirinto[i][j].visitado = false;
            labirinto[i][j].parede_norte = true;
            labirinto[i][j].parede_sul = true;
            labirinto[i][j].parede_oeste = true;
            labirinto[i][j].parede_leste = true;
            labirinto[i][j].item = ITEM_NENHUM; 
        }
    }
}


void GeradorLabirinto(int x, int y) {
    //Declaração de variáveis
    labirinto[x][y].visitado = true;
    int direcoes[] = {0, 1, 2, 3}; //Norte, Leste, Sul, Oeste
    //Randomizando as direções a seguir
    for (int i = 0; i < 4 ; i++) {
        int j = GetRandomValue(i, 3);
        int temp = direcoes[i];
        direcoes[i] = direcoes[j];
        direcoes[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x;
        int ny = y;
        if (direcoes[i] == 0) {
            ny = y - 1; 
        } else if (direcoes[i] == 1) {
            nx = x + 1; 
        } else if (direcoes[i] == 2) {
            ny = y + 1; 
        } else if (direcoes[i] == 3) {
            nx = x - 1;    //Oeste    
        }

        if (nx >= 0 && nx < GRADE_COMP && ny >= 0 && ny < GRADE_ALTURA  && !labirinto[nx][ny].visitado) {
            if (direcoes[i] == 0 ) {
                labirinto[x][y].parede_norte = false;
                labirinto[nx][ny].parede_sul = false;

            } else if (direcoes[i] == 1) {
                labirinto[x][y].parede_leste = false;
                labirinto[nx][ny].parede_oeste = false;

            } else if (direcoes[i] == 2) {
                labirinto[x][y].parede_sul = false;
                labirinto[nx][ny].parede_norte = false;

            } else if (direcoes[i] == 3) {
                labirinto[x][y].parede_oeste = false;
                labirinto[nx][ny].parede_leste = false;

            }
            GeradorLabirinto(nx,ny); //Chama recursiva
        }
    }



}

void DesenhoVisaoJogador() {
    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) {
            int dist_x = abs(x - jogador.x);
            int dist_y = abs(y - jogador.y);

            if (dist_x <= RAIO_LUZ && dist_y <= RAIO_LUZ) {
                // Célula visível: desenha as paredes
                int posicao_x = x * TAMANHO_CELULA;
                int posicao_y = y * TAMANHO_CELULA;

                if (labirinto[x][y].item != ITEM_NENHUM) {
                    Texture2D textura_desenho = {0};
                    
                    switch (labirinto[x][y].item) {
                        case ITEM_MOEDA:
                            textura_desenho = textura_moeda;
                            break;
                        case ITEM_CRISTAL:
                            textura_desenho = textura_cristal; 
                            break;
                        case ITEM_CHAVE:
                            textura_desenho = textura_chave;
                            break;
                        default: break;
                    }
                    if (textura_desenho.id != 0) { // Desenha se a textura carregou
                        
                        const float FATOR_ESCALA = 0.8f; // Item um pouco menor que a célula 
                        
                        float largura_desenho = (float)TAMANHO_CELULA * FATOR_ESCALA;
                        float altura_desenho = (float)TAMANHO_CELULA * FATOR_ESCALA;
                        
                        // Calcula o ponto de início para centralizar o desenho
                        float pos_inicio_x = posicao_x + (TAMANHO_CELULA - largura_desenho) / 2.0f;
                        float pos_inicio_y = posicao_y + (TAMANHO_CELULA - altura_desenho) / 2.0f;
                        
                        Rectangle sourceRect = { 0.0f, 0.0f, (float)textura_desenho.width, (float)textura_desenho.height };
                        Rectangle destRect = { pos_inicio_x, pos_inicio_y, largura_desenho, altura_desenho };
                        
                        DrawTexturePro(textura_desenho, sourceRect, destRect, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
                    }
                    
                }
                if (labirinto[x][y].parede_norte) DrawLine(posicao_x, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y, WHITE);
                if (labirinto[x][y].parede_leste) DrawLine(posicao_x + TAMANHO_CELULA, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_sul) DrawLine(posicao_x, posicao_y + TAMANHO_CELULA, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_oeste) DrawLine(posicao_x, posicao_y, posicao_x, posicao_y + TAMANHO_CELULA, WHITE);

                if (x == Objetivo.x && y == Objetivo.y) {
                    DrawRectangle(Objetivo.x * TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
                }
            }

        }

    }
}


void DesenhoNevoa() {

    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) {
            //Calculo a distância euclidiana
            float dist_x_float = (float)abs(x - jogador.x);
            float dist_y_float = (float)abs(y - jogador.y);
            float distancia = sqrtf(dist_x_float * dist_x_float + dist_y_float * dist_y_float);
            // ponto onde a névoa começa
            const float RAIO_VISIVEL = (float)RAIO_LUZ; 
            const float RAIO_MAXIMO = (float) MAX_VISAO; 
            float alpha = 0.0f;

            if (distancia >= RAIO_MAXIMO) {
                //Opacidade máxima
                alpha = 1.0f;
            } else if (distancia > RAIO_VISIVEL) {
                //Zona de Transição
                alpha = (distancia - RAIO_VISIVEL) / (RAIO_MAXIMO - RAIO_VISIVEL);
            }
            if (alpha > 0.01f) { 
                int posicao_x = x * TAMANHO_CELULA;
                int posicao_y = y * TAMANHO_CELULA;
                //Desenho da cor preta com a opacidade calculada
                DrawRectangle(posicao_x, posicao_y, TAMANHO_CELULA, TAMANHO_CELULA, Fade(BLACK, alpha));
            }
        }
    }
}



void DesenhoJogador() {
    //Posição central da célula onde o jogador está
    float centro_x_celula = posicaoDesenhoJogador.x + TAMANHO_CELULA / 2.0f;
    float centro_y_celula = posicaoDesenhoJogador.y + TAMANHO_CELULA / 2.0f;

    //Tamanho desejado do desenho 
    float largura_desenho = (float)TAMANHO_CELULA; 
    float altura_desenho = (float)TAMANHO_CELULA; 

    const int escala = 2.0;
    largura_desenho *= escala;
    altura_desenho *= escala;

    Rectangle sourceRect = { 0.0f, 0.0f, (float)textura_jogador.width, (float)textura_jogador.height };

    Rectangle destRect = { 
        centro_x_celula - largura_desenho / 2.0f, 
        centro_y_celula - altura_desenho / 2.0f,  
        largura_desenho, 
        altura_desenho 
    };
    
    DrawTexturePro(textura_jogador, sourceRect, destRect, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}
void DesenhoTubarao() {

    float centro_x_celula = posicaoDesenhoTubarao.x + TAMANHO_CELULA / 2.0f;
    float centro_y_celula = posicaoDesenhoTubarao.y + TAMANHO_CELULA / 2.0f;

    float largura_desenho = (float)TAMANHO_CELULA;
    float altura_desenho = (float)TAMANHO_CELULA;
    const float escala = 3.0;
    largura_desenho *= escala;
    altura_desenho *= escala;


    Rectangle sourceRect = { 0.0f, 0.0f, (float)textura_tubarao.width, (float)textura_tubarao.height };

    Rectangle destRect = { 
        centro_x_celula - largura_desenho / 2.0f, 
        centro_y_celula - altura_desenho / 2.0f,  
        largura_desenho, 
        altura_desenho 
    };

    Vector2 origin = { 0.0f, 0.0f }; 

    DrawTexturePro(textura_tubarao, sourceRect, destRect, origin, 0.0f, WHITE);

}
void DesenhoObjetivo() {
    DrawRectangle(Objetivo.x *TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
}


void MoverJogador(int dx, int dy) {
    int novoX = jogador.x + dx;
    int novoY = jogador.y + dy;
    if (novoX < 0 || novoX >= GRADE_COMP || novoY < 0 || novoY >= GRADE_ALTURA) return;
    if (dx == -1 && labirinto[jogador.x][jogador.y].parede_oeste) return;
    if (dx == 1 && labirinto[jogador.x][jogador.y].parede_leste) return;
    if (dy == 1 && labirinto[jogador.x][jogador.y].parede_sul) return;
    if (dy == -1 && labirinto[jogador.x][jogador.y].parede_norte) return;
    jogador.x = novoX;
    jogador.y = novoY;
}

void ColocaItensAleatorios() {
    //Definição da quantidade de cada item
    const int NUM_MOEDAS = 15;
    const int NUM_CRISTAIS = 8;
    const int NUM_CHAVES = 3; 
    
    int totalItens = NUM_MOEDAS + NUM_CRISTAIS + NUM_CHAVES;
    
    //Limite de itens para não preencher demais
    if (totalItens >= GRADE_COMP * GRADE_ALTURA - 2) {
        totalItens = GRADE_COMP * GRADE_ALTURA - 3; 
    }

    int itensColocados = 0;

    //loop de posicionamento
    while (itensColocados < totalItens) {
        //escolhe coordenadas aleatórias
        int x = GetRandomValue(0, GRADE_COMP - 1);
        int y = GetRandomValue(0, GRADE_ALTURA - 1);

        //pula se a posição for a de início ou a do objetivo
        if ((x == 0 && y == 0) || (x == Objetivo.x && y == Objetivo.y)) continue;
        
        //verifica se a célula está vazia e atribui o item
        if (labirinto[x][y].item == ITEM_NENHUM) {
            TipoItem novoItem = ITEM_NENHUM;
            
            // Prioridade de colocação: Moedas -> Cristais -> Chaves
            if (itensColocados < NUM_MOEDAS) {
                novoItem = ITEM_MOEDA;
            } else if (itensColocados < NUM_MOEDAS + NUM_CRISTAIS) {
                novoItem = ITEM_CRISTAL;
            } else {
                novoItem = ITEM_CHAVE;
            }
            
            //atribui o item e conta o total
            labirinto[x][y].item = novoItem;
            itensColocados++;
        }
    }
}

void ColetaItem() {
    int x = (int)round(posicaoDesenhoJogador.x / TAMANHO_CELULA);
    int y = (int)round(posicaoDesenhoJogador.y / TAMANHO_CELULA);
    
    //Garantia de que a posição está dentro da grade
    if (x < 0 || x >= GRADE_COMP || y < 0 || y >= GRADE_ALTURA) return;

    TipoItem itemColetado = labirinto[x][y].item;

    if (itemColetado != ITEM_NENHUM) {
        PlaySound(som_coleta);

        switch (itemColetado) {
            case ITEM_MOEDA: placar.moedas++; break;
            case ITEM_CRISTAL: placar.cristais++; break;
            case ITEM_CHAVE: placar.chaves++; break;
            default: break;
        }
        //Remove o item da célula
        labirinto[x][y].item = ITEM_NENHUM; 
    }
}

void DesenhaPlacar() {
    //Definições
    const int LARGURA_PLACAR = 240; 
    int startY = 10;
    int fontSize = 20;
    
    //Cálculo da Posição (Canto Superior Direito)
    int comprimento_tela = (int)TELA_COMP;
    int inicioX = comprimento_tela - LARGURA_PLACAR - 10;
    int textoX = inicioX + 10;

    //Fundo do placar
    DrawRectangle(inicioX, 0, LARGURA_PLACAR, 100, Fade(DARKGRAY, 0.7f)); 
    
    //Desenho dos Itens
    //Moedas
    DrawText(TextFormat("MOEDAS: %d / %d", placar.moedas, meta_vitoria.moedas_necessarias),textoX, startY, fontSize, YELLOW);
    
    //Cristais 
    DrawText(TextFormat("CRISTAIS: %d / %d", placar.cristais, meta_vitoria.cristais_necessarios),textoX, startY + fontSize + 5, fontSize,VIOLET);
    
    //Chaves
    DrawText(TextFormat("CHAVES: %d / %d", placar.chaves, meta_vitoria.chaves_necessarias), textoX, startY + 2 * (fontSize + 5), fontSize,GOLD);
}


int fase1() {
    // Inicializa labirinto só uma vez
    static bool iniciado = false;
    
    // 1. Inicializa a Janela com as dimensões da GRADE
    if (!IsWindowReady()) {
        InitWindow(TELA_COMP, TELA_ALTURA, "Seu Labirinto com Placar");
        SetTargetFPS(60); 
    }    
    //Inicialização da Lógica do Jogo
    if (!iniciado) {
        CarregarRecursosFase1();
        InicializaLabirinto();
        GeradorLabirinto(0,0);
        jogador.x = 0;
        jogador.y = 0;

        posicaoDesenhoJogador.x = jogador.x * TAMANHO_CELULA;
        posicaoDesenhoJogador.y = jogador.y * TAMANHO_CELULA;

        tubarao.x = GRADE_COMP / 2;
        tubarao.y = GRADE_ALTURA / 2;

        posicaoDesenhoTubarao.x = tubarao.x * TAMANHO_CELULA;
        posicaoDesenhoTubarao.y = tubarao.y * TAMANHO_CELULA;

        //Definimos a velocidade inicial para começar a se mover 

        velocidadeDesenhoTubarao.x = VELOCIDADE_TUBARAO_CONTINUO;

        velocidadeDesenhoTubarao.y = VELOCIDADE_TUBARAO_CONTINUO;


        placar.moedas = 0;
        placar.cristais = 0;
        placar.chaves = 0;
    
        //Definimos a meta de coleta para vitória
        meta_vitoria.moedas_necessarias = META_MOEDAS; 
        meta_vitoria.cristais_necessarios = META_CRISTAIS; 
        meta_vitoria.chaves_necessarias = META_CHAVES;    
        ColocaItensAleatorios();

        iniciado = true;
        Objetivo.x = GRADE_COMP -1;
        Objetivo.y = GRADE_ALTURA - 1;

    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (IsWindowReady()) CloseWindow(); 
        return STATE_MENU;
    }

    bool movimentoCompleto = (posicaoDesenhoJogador.x == (float)jogador.x * TAMANHO_CELULA) && (posicaoDesenhoJogador.y == (float)jogador.y * TAMANHO_CELULA);
    //Restringe o movimento
    if (movimentoCompleto) {
        if (IsKeyDown(KEY_RIGHT)) {
            MoverJogador(1,0);
        } else if (IsKeyDown(KEY_LEFT)) { 
            MoverJogador(-1,0);
        } else if (IsKeyDown(KEY_UP)) { 
            MoverJogador(0,-1);
        } else if (IsKeyDown(KEY_DOWN)) {
            MoverJogador(0,1);
        }

        ColetaItem();
    }

    float destinoX = (float)jogador.x * TAMANHO_CELULA;
    float destinoY = (float)jogador.y * TAMANHO_CELULA;
   
    float delta = GetFrameTime();

    if (posicaoDesenhoJogador.x != destinoX) {
        if (posicaoDesenhoJogador.x < destinoX) {
            posicaoDesenhoJogador.x += VELOCIDADE_JOGADOR * delta;
            if (posicaoDesenhoJogador.x > destinoX) posicaoDesenhoJogador.x = destinoX;

        } else {
            posicaoDesenhoJogador.x -= VELOCIDADE_JOGADOR * delta;
            if (posicaoDesenhoJogador.x < destinoX) posicaoDesenhoJogador.x = destinoX;

        }

    }

    if (posicaoDesenhoJogador.y != destinoY) {
        if (posicaoDesenhoJogador.y < destinoY) {
            posicaoDesenhoJogador.y += VELOCIDADE_JOGADOR * delta;
            if (posicaoDesenhoJogador.y > destinoY) posicaoDesenhoJogador.y = destinoY;

        } else {
            posicaoDesenhoJogador.y -= VELOCIDADE_JOGADOR * delta;
            if (posicaoDesenhoJogador.y < destinoY) posicaoDesenhoJogador.y = destinoY;

        }
    }

    posicaoDesenhoTubarao.x += velocidadeDesenhoTubarao.x * delta;
    posicaoDesenhoTubarao.y += velocidadeDesenhoTubarao.y * delta;


    //Lógica de Rebate nas Bordas da Tela
    //Rebate no eixo X
    if (posicaoDesenhoTubarao.x < 0) {
        velocidadeDesenhoTubarao.x *= -1; 
        posicaoDesenhoTubarao.x = 0;      

    } else if (posicaoDesenhoTubarao.x + TAMANHO_CELULA > TELA_COMP) {
        velocidadeDesenhoTubarao.x *= -1;
        posicaoDesenhoTubarao.x = TELA_COMP - TAMANHO_CELULA; 

    }

    // Rebate no eixo Y (altura)
    if (posicaoDesenhoTubarao.y < 0) {
        velocidadeDesenhoTubarao.y *= -1; 
        posicaoDesenhoTubarao.y = 0;

    } else if (posicaoDesenhoTubarao.y + TAMANHO_CELULA > TELA_ALTURA) {
        velocidadeDesenhoTubarao.y *= -1; 
        posicaoDesenhoTubarao.y = TELA_ALTURA - TAMANHO_CELULA;

    }

    bool meta_coletada = (placar.moedas >= meta_vitoria.moedas_necessarias &&placar.cristais >= meta_vitoria.cristais_necessarios && placar.chaves >= meta_vitoria.chaves_necessarias);

    if (jogador.x == Objetivo.x && jogador.y == Objetivo.y && meta_coletada) {
        iniciado = false;
        PlaySound(som_game_win);
        return STATE_MENU;
    }

    tubarao.x = (int)round(posicaoDesenhoTubarao.x / TAMANHO_CELULA);
    tubarao.y = (int)round(posicaoDesenhoTubarao.y / TAMANHO_CELULA);

    if (VerificarColisao()) {
        iniciado = false;
        PlaySound(som_game_over);
        //se houver colisão, retorna o novo estado de gameover
        return STATE_GAMEOVER; 
    }

    // Desenho da fase
    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle src = { 0.0f, 0.0f, (float)fundo_fase1.width, (float)fundo_fase1.height };
        Rectangle dest = { 0.0f, 0.0f, (float)TELA_COMP, (float)TELA_ALTURA }; 
        DrawTexturePro(fundo_fase1,src,dest,(Vector2){0, 0},0.0f,WHITE);

        DesenhoVisaoJogador();
        DesenhoJogador();
        DesenhoTubarao();
        DesenhoNevoa();
        DesenhoObjetivo();
        DesenhaPlacar();
        
    EndDrawing();
    
    return STATE_FASE1;

}