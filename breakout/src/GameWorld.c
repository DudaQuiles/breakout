/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#include "Bolinha.h"
#include "GameWorld.h"
#include "Jogador.h"
#include "ResourceManager.h"

/**
 * Resolve a colisão entre a bolinha "b" com os alvos da partida.
 * Ao colidir com um alvo a bolinha é reposicionada, o alvo em questão
 * perde um ponto de hp e o jogador deve ganhar alguma quantidade de pontos.
 */
void resolverColisaoBolinhaAlvos(Bolinha *b, Jogador *j, Alvo *alvos, int quantidade);
void resolverColisaoBolaJogador( Bolinha *b, Jogador *j );
void desenharEstado(Jogador *j);
void jogoPausado(Bolinha *b, Jogador *j);
void gameOver(Jogador *j);
void gameWin(void);
int pontuacao = 0;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld *createGameWorld(void){

    GameWorld *gw = (GameWorld*) malloc(sizeof(GameWorld));

    int larguraJogador = 150;
    int alturaJogador = 20;

    gw->jogador = (Jogador){
        .ret = { 
            GetScreenWidth() / 2 - larguraJogador / 2, 
            GetScreenHeight() - 3 * alturaJogador, 
            larguraJogador, 
            alturaJogador
        },
        .velocidadeBase = 200,
        .velocidadeAtual = 0,
        .cor = WHITE,
        .vida = 3,
        .pontuacao = 0,
        .estado = 0
    };

    gw->bolinha = (Bolinha){
        .centro = { 
            GetScreenWidth() / 2,
            gw->jogador.ret.y - gw->jogador.ret.width
        },
        .raio = 10,
        .vel = { 200, -200 },
        .cor = WHITE
    };

    gw->lin = 10;
    gw->col = 6;

    // cria um array de alvos de lin * col (nesse caso, 60 alvos)
    gw->alvos = (Alvo*) malloc(sizeof(Alvo) * gw->lin * gw->col);

    Color coresAlvos[] = {
        { 100, 200, 50, 255 },     // { vermelho, verde, azul, alpha }
        GetColor( 0x0099CCFF ), // GetColor( cor em hexadecimal )
        BLUE,                   // constante para cor
        GREEN,
        ORANGE,
        RED,
        WHITE,
        PURPLE,
        YELLOW,
        DARKGREEN
    };

    int larguraAlvo = 80;
    int alturaAlvo = 20;
    int espaco = 5;
    int larguraTotal = larguraAlvo * gw->col + espaco * ( gw->col - 1 );
    int xIni = GetScreenWidth() / 2 - larguraTotal / 2;
    int yIni = 150;

    for (int i = 0; i < gw->lin; i++){
        for (int j = 0; j < gw->col; j++){

            // calculo da posição linear de um elemento da grade linhas x colunas
            // dentro do array de alvos
            int p = i * gw->col + j;

            gw->alvos[p] = (Alvo){
                .ret = {
                    .x = xIni + j * ( larguraAlvo + espaco ), // cálculo da posição horizontal (depende da coluna atual)
                    .y = yIni + i * ( alturaAlvo + espaco ),  // cálculo da posição vertical (depende da linha atual)
                    .width = larguraAlvo,
                    .height = alturaAlvo,
                },
                .cor = coresAlvos[i], // cuidado aqui...
                .hp = 1
            };

        }
    }

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld(GameWorld *gw){
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld(GameWorld *gw, float delta){
    if (gw->jogador.estado == 0){
        jogoPausado(&gw->bolinha,&gw->jogador);
    }else if (gw->jogador.vida == 0){
        gameOver(&gw->jogador);
    }else{
        entradaJogador( &gw->jogador );
        atualizarJogador( &gw->jogador, delta );
        atualizarBolinha( &gw->bolinha, &gw->jogador, delta );
        resolverColisaoBolinhaAlvos( &gw->bolinha, &gw->jogador, gw->alvos, gw->lin * gw->col );
        resolverColisaoBolaJogador( &gw->bolinha, &gw->jogador );
    }
    
}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld(GameWorld *gw){

    BeginDrawing();
    ClearBackground(BLACK);

    desenharJogador(&gw->jogador);
    desenharBolinha(&gw->bolinha);
    desenharAlvos(gw->alvos, gw->lin * gw->col);
    desenharVidaPlacar(&gw->jogador);

    EndDrawing();

}

void resolverColisaoBolinhaAlvos(Bolinha *b, Jogador *j, Alvo *alvos, int quantidade){

    for (int i = 0; i < quantidade; i++){

        // para cada alvo
        Alvo *alvo = &alvos[i];

        // verifica se ainda tem vida (hp > 0) e se a bolinha colidiu com seu retângulo
        if (alvo->hp > 0 && CheckCollisionCircleRec(b->centro, b->raio, alvo->ret)){

            // perde um ponto de vida
            alvo->hp--;
            j->pontuacao++; 

            // reposicionamento e espelhamento apropriado da velocidade da bolinha
            
            // coordenada do centro do alvo
            float centroAlvoX = alvo->ret.x + alvo->ret.width  / 2.0f;
            float centroAlvoY = alvo->ret.y + alvo->ret.height / 2.0f;

            // calcula a sobreposição (penetração) da bolinha em cada eixo
            // o menor overlap indica por qual face a bolinha entrou de fato
            float overlapX = (b->raio + alvo->ret.width  / 2.0f) - fabs(b->centro.x - centroAlvoX);
            float overlapY = (b->raio + alvo->ret.height / 2.0f) - fabs(b->centro.y - centroAlvoY);

            if (overlapX < overlapY){

                // colisão lateral (esquerda ou direita): empurra em X e espelha vel.x
                if (b->centro.x < centroAlvoX){
                    b->centro.x = alvo->ret.x - b->raio;                    // sai pela esquerda
                } else{
                    b->centro.x = alvo->ret.x + alvo->ret.width + b->raio;  // sai pela direita
                }
                b->vel.x = -b->vel.x;

            } else{

                // colisão topo/base: empurra em Y e espelha vel.y
                if ( b->centro.y < centroAlvoY ) {
                    b->centro.y = alvo->ret.y - b->raio;                     // sai por cima
                } else {
                    b->centro.y = alvo->ret.y + alvo->ret.height + b->raio;  // sai por baixo
                }
                b->vel.y = -b->vel.y;

            }

        }
    }

}

void desenharEstado(Jogador *j){
    if(j->estado == 0){
        int tamanhoFonte = 40;
        DrawText("Aperte para continuar", GetScreenWidth() / 2, GetScreenHeight() / 2, tamanhoFonte, WHITE);
    }
}

void gameOver(Jogador *j){
    int tamanhoFonte = 40;
    const char *textoOver = TextFormat("Você perdeu, aperte espaço para tentar de novo: ");
    int t = MeasureText(textoOver,tamanhoFonte);
    DrawText(textoOver, GetScreenWidth()-t-10, GetScreenHeight() / 2, tamanhoFonte, WHITE);
    if( IsKeyPressed(KEY_SPACE)){
        j->vida = 3;
        j->pontuacao = 0;
        j->estado = 0;

    }
}
void resolverColisaoBolaJogador( Bolinha *b, Jogador *j ) {

    if ( CheckCollisionCircleRec( b->centro, b->raio, j->ret ) ) {
        b->centro.y = j->ret.y - b->raio;
        b->vel.y = -b->vel.y;
    }
}
void jogoPausado(Bolinha *b, Jogador *j){
    if(IsKeyPressed(KEY_LEFT)){
        b->vel.x = -200;
        b->vel.y = -200;
        j->estado = 1;
    }
    if (IsKeyPressed(KEY_RIGHT)){
        b->vel.x = 200;
        b->vel.y = -200;
        j->estado = 1;
    }
}