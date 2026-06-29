#include "raylib/raylib.h"
#include "Alvo.h"

/**
 * Desenha um alvo.
 */
void desenharAlvo( Alvo *alvo ) {
    if ( alvo->hp > 0 ) {
        DrawRectangleRec( alvo->ret, alvo->cor );
    }
}

/**
 * Desenha todos os alvos de um array que tem tamanho igual a quantidade.
 */
void desenharAlvos( Alvo *alvos, int quantidade ) {
    for ( int i = 0; i < quantidade; i++ ) {
        desenharAlvo( &alvos[i] );
    }
}
