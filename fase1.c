#include "raylib.h"
#include <stdio.h>  
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "fase1.h"

Celula labirinto[GRADE_COMP][GRADE_ALTURA];
Posicao jogador;
Posicao Objetivo;
PosicaoFloat posicaoDesenhoJogador;
Posicao tubarao;
PosicaoFloat posicaoDesenhoTubarao;
PosicaoFloat velocidadeDesenhoTubarao = {0};
Texture2D fundo_fase1 = {0};

//Carregar Imagem
void CarregarFundo1() {
    fundo_fase1 = LoadTexture("data/fundo_fase1.jpg");
}

//Criamos a grade toda
void InicializaLabirinto() {
    for(int i = 0;i < GRADE_COMP; i++) {
        for(int j = 0; j < GRADE_ALTURA; j++) {
            labirinto[i][j].visitado = false;
            labirinto[i][j].parede_norte = true;
            labirinto[i][j].parede_sul = true;
            labirinto[i][j].parede_oeste = true;
            labirinto[i][j].parede_leste = true;

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
    //Loop das direções a seguir
    for (int i = 0; i < 4; i++) {
        int nx = x; 
        int ny = y;
        if (direcoes[i] == 0) {
            ny = y - 1;    //Norte
        } else if (direcoes[i] == 1) {
            nx = x + 1;    //Leste
        } else if (direcoes[i] == 2) {
            ny = y + 1;    //Sul
        } else if (direcoes[i] == 3) {
            nx = x - 1;    //Oeste     
        }
        //Teste se a célula é válida (Não já foi visitada nem passa dos limites da grade)
        if (nx >= 0 && nx < GRADE_COMP && ny >= 0 && ny < GRADE_ALTURA  && !labirinto[nx][ny].visitado) {
            //Tira as paredes entre a célula e a próxima
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
            GeradorLabirinto(nx,ny); //Chamada recursiva
        }
    }

}
void DesenhoVisaoJogador() {
    //Percorremos todas as coordenadas possíveis
    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) {
            //Calculo de distancia entre coordenada e jogador
            int dist_x = abs(x - jogador.x);
            int dist_y = abs(y - jogador.y);
            
            //Condição de Visibilidade
            if (dist_x <= RAIO_LUZ && dist_y <= RAIO_LUZ) {
                
                //Determine as posições das paredes
                int posicao_x = x * TAMANHO_CELULA;
                int posicao_y = y * TAMANHO_CELULA;
                //Desenha as paredes baseado nas coordenadas visiveis
                if (labirinto[x][y].parede_norte) DrawLine(posicao_x, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y, WHITE);
                if (labirinto[x][y].parede_leste) DrawLine(posicao_x + TAMANHO_CELULA, posicao_y, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_sul) DrawLine(posicao_x, posicao_y + TAMANHO_CELULA, posicao_x + TAMANHO_CELULA, posicao_y + TAMANHO_CELULA, WHITE);
                if (labirinto[x][y].parede_oeste) DrawLine(posicao_x, posicao_y, posicao_x, posicao_y + TAMANHO_CELULA, WHITE);

                //Objeto
                if (x == Objetivo.x && y == Objetivo.y) {
                    DrawRectangle(Objetivo.x * TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
                }
            }
        }
    }
    
}

//Lógica do círculo de luz
void DesenhoNevoa() {
    for (int x = 0; x < GRADE_COMP; x++) {
        for (int y = 0; y < GRADE_ALTURA; y++) {
            
            //Cálculo da distância real (euclidiana)
            float dist_x_float = (float)abs(x - jogador.x);
            float dist_y_float = (float)abs(y - jogador.y);
            float distancia = sqrtf(dist_x_float * dist_x_float + dist_y_float * dist_y_float);
            
            //Definimos o ponto onde a névoa começa
            const float RAIO_VISIVEL = (float)RAIO_LUZ; //Casting para float
            const float RAIO_MAXIMO = (float) MAX_VISAO; 

            //Isso aqui é a opacidade
            float opacidade = 0.0f;

            if (distancia >= RAIO_MAXIMO) {
                //Muito longe (Totalmente Escuro)
                opacidade = 1.0f;
            } else if (distancia > RAIO_VISIVEL) {
                //Zona de Transição: Calculamos a opacidade com base na posição dos dois raios
                opacidade = (distancia - RAIO_VISIVEL) / (RAIO_MAXIMO - RAIO_VISIVEL);
            }
            //Se a distância for menor ou igual a RAIO_VISIVEL, opacidade é 0.0f (totalmente transparente)

            //Desenho do raio(névoa)
            if (opacidade > 0.01f) { //Desenhamos o retangulo apenas se houve opacidade
                int posicao_x = x * TAMANHO_CELULA;
                int posicao_y = y * TAMANHO_CELULA;
                
                //Desenho do retângulo com base na opacidade
                DrawRectangle(posicao_x, posicao_y, TAMANHO_CELULA, TAMANHO_CELULA, Fade(BLACK, opacidade));
            }
        }
    }
}

//Desenho dos objetos
void DesenhoJogador() {
   DrawRectangle((int)posicaoDesenhoJogador.x + 2, (int)posicaoDesenhoJogador.y + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, WHITE);
}

void DesenhoTubarao() {
    DrawRectangle((int)posicaoDesenhoTubarao.x + 2, (int)posicaoDesenhoTubarao.y + 2, (int)TAMANHO_CELULA - 4, (int)TAMANHO_CELULA - 4, RED);
}

void DesenhoObjetivo() {
    DrawRectangle(Objetivo.x *TAMANHO_CELULA + 2, Objetivo.y * TAMANHO_CELULA + 2, TAMANHO_CELULA - 4, TAMANHO_CELULA - 4, BLUE);
}
//Trata da movimentação do jogador
void MoverJogador(int dx, int dy) {
    //Coordenadas baseadas nas direções de input
    int novoX = jogador.x + dx;
    int novoY = jogador.y + dy;
    //Condição de não ter parede na direção requisitada
    if (novoX < 0 || novoX >= GRADE_COMP || novoY < 0 || novoY >= GRADE_ALTURA) return;
    if (dx == -1 && labirinto[jogador.x][jogador.y].parede_oeste) return;
    if (dx == 1 && labirinto[jogador.x][jogador.y].parede_leste) return;
    if (dy == 1 && labirinto[jogador.x][jogador.y].parede_sul) return;
    if (dy == -1 && labirinto[jogador.x][jogador.y].parede_norte) return;

    //Atualiza a posição do jogador
    jogador.x = novoX;
    jogador.y = novoY;
}
//Verificação de Colisão entre o jogador e o tubarão
bool verificarColisao() {
    if (jogador.x == tubarao.x && jogador.y == tubarao.y) {
        return true;
    }
    return false;   
}
int fase1() {
    // Inicializa labirinto só uma vez
    static bool iniciado = false;
    if (!iniciado) {
        // Inicializa labirinto só uma vez
        InicializaLabirinto();
        GeradorLabirinto(0,0);
        //Coloca jogador na posição (0,0)
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

        iniciado = true;
        //Objetivo: vamos tirar depois
        Objetivo.x = GRADE_COMP -1;
        Objetivo.y = GRADE_ALTURA - 1;

    }
    //Se quiser sair da fase 1
    if (IsKeyPressed(KEY_ESCAPE)) {
        return STATE_MENU;
    }
    //Evita que o jogador se mova em na diagonal de uma vez
    bool movimentoCompleto = (posicaoDesenhoJogador.x == (float)jogador.x * TAMANHO_CELULA) && (posicaoDesenhoJogador.y == (float)jogador.y * TAMANHO_CELULA);

    if (movimentoCompleto) {
        if (IsKeyDown(KEY_RIGHT)) {
            MoverJogador(1,0);
        } else if (IsKeyDown(KEY_LEFT)) { // Use 'else if' para garantir apenas um movimento
            MoverJogador(-1,0);
        } else if (IsKeyDown(KEY_UP)) { // Use 'else if'
            MoverJogador(0,-1);
        } else if (IsKeyDown(KEY_DOWN)) { // Use 'else if'
            MoverJogador(0,1);
    }
    }

    float destinoX = (float)jogador.x * TAMANHO_CELULA;
    float destinoY = (float)jogador.y * TAMANHO_CELULA;
    
    float delta = GetFrameTime();
    
    //Suave o movimento
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

    //Lógica de Rebate nas bordas da tela
    if (posicaoDesenhoTubarao.x < 0) {
        velocidadeDesenhoTubarao.x *= -1; 
        posicaoDesenhoTubarao.x = 0;      
    } else if (posicaoDesenhoTubarao.x + TAMANHO_CELULA > TELA_COMP) {
        velocidadeDesenhoTubarao.x *= -1;
        posicaoDesenhoTubarao.x = TELA_COMP - TAMANHO_CELULA; 
    }
    //Lógica de Rebate nas bordas da tela
    if (posicaoDesenhoTubarao.y < 0) {
        velocidadeDesenhoTubarao.y *= -1; 
        posicaoDesenhoTubarao.y = 0;
    } else if (posicaoDesenhoTubarao.y + TAMANHO_CELULA > TELA_ALTURA) {
        velocidadeDesenhoTubarao.y *= -1; 
        posicaoDesenhoTubarao.y = TELA_ALTURA - TAMANHO_CELULA;
    }

    //VERIFICAÇÃO DE COLISÃO    
    //Garantimos que as coordenadas do tubarao sao inteiras após rebate
    tubarao.x = (int)round(posicaoDesenhoTubarao.x / TAMANHO_CELULA);
    tubarao.y = (int)round(posicaoDesenhoTubarao.y / TAMANHO_CELULA);

    if (verificarColisao()) {
        // Se houver colisão, retorna o novo estado de Game Over
        iniciado = false;
        return STATE_GAME_OVER; 
    }



    // Desenho da fase
    BeginDrawing();
        //Chamada das fuções de desenho
        ClearBackground(BLACK);

        Rectangle src = { 0.0f, 0.0f, (float)fundo_fase1.width, (float)fundo_fase1.height };
        Rectangle dest = { 0.0f, 0.0f, (float)TELA_COMP, (float)TELA_ALTURA }; 

        DrawTexturePro(fundo_fase1,src,dest,(Vector2){0, 0},0.0f,WHITE);

        DesenhoVisaoJogador();
        DesenhoJogador();
        DesenhoTubarao();
        DesenhoNevoa();
        DesenhoObjetivo();
        
    EndDrawing();

    return STATE_FASE1;
}