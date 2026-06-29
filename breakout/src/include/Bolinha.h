/**
 * Definição do tipo Bolinha e operações associadas.
 * 
 * Uma Bolinha é usada no jogo com o objetivo de quebrar os alvos e ser
 * rebatida pelo jogador.
 */
#pragma once

#include "raylib/raylib.h"
#include "Jogador.h"

typedef struct Bolinha {
    Vector2 centro;
    float raio;
    Vector2 vel;
    Color cor;
} Bolinha;

void atualizarBolinha( Bolinha *bolinha,Jogador *j, float delta );
void desenharBolinha( Bolinha *bolinha );
void pausarJogo(Bolinha *bolinha);
