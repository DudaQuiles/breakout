/**
 * Definição do tipo Jogador e operações associadas.
 * 
 * Um Jogador representa, obviamente, o jogador do Breakout, que tem como
 * objetivo quebrar todos os alvos contidos na tela.
 */
#pragma once

#include "raylib/raylib.h"

typedef struct Jogador{
    Rectangle ret;
    float velocidadeBase;    // a velocidade que será usada na movimentação (não é alterada)
    float velocidadeAtual;   // a velocidade usada no momento
    Color cor;
    int vida;
    int pontuacao;
    int estado;
}Jogador;

void entradaJogador(Jogador *j);
void atualizarJogador(Jogador *j, float delta);
void desenharJogador(Jogador *j);
void desenharVidaPlacar(Jogador *j);
