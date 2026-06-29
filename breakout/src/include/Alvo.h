/**
 * Definição do tipo Alvo e operações associadas.
 * 
 * Um Alvo representa o elemento do jogo que pode ser acertado pela bolinha,
 * devendo ser quebrado pelo jogador.
 */
#pragma once

#include "raylib/raylib.h"

typedef struct Alvo {
    Rectangle ret;
    Color cor;        
    int hp;           // health points (pontos de vida)
} Alvo;

void desenharAlvo( Alvo *alvo );
void desenharAlvos( Alvo *alvos, int quantidade );