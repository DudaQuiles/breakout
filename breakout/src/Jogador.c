#include <stdlib.h>

#include "raylib/raylib.h"

#include "Jogador.h"

/**
 * Processa e entrada associada ao jogador.
 */
void entradaJogador( Jogador *j ) {

    // armazena -1 caso a tecla seta para a esquerda estiver pressionada ou 0 caso contrário
    int esquerda = IsKeyDown( KEY_LEFT )  ? -1 : 0;

    // armazena 1 caso a tecla seta para a direita estiver pressionada ou 0 caso contrário
    int direita  = IsKeyDown( KEY_RIGHT ) ?  1 : 0;

    // compõe os dois movimentos (estado das teclas)
    int movimento = esquerda + direita;

    // utiliza a composição do movimento para calcular a velocidade atual
    // usada para movimentar o jogador
    j->velocidadeAtual = j->velocidadeBase * movimento;

}

/**
 * Atualiza a posição do jogador e resolve a colisão com as extremidades da tela.
 */
void atualizarJogador( Jogador *j, float delta ) {
    // integração de Euler
    if(j->vida > 0){
        j->ret.x += j->velocidadeAtual * delta;

    // verificação e resolução de colisão com as bordas horizontais da janela
        if ( j->ret.x <= 0 ) {
            j->ret.x = 0;
        } else if ( j->ret.x + j->ret.width >= GetScreenWidth() ) {
            j->ret.x = GetScreenWidth() - j->ret.width;
        }
    }

}

/**
 * Desenha um jogador.
 */
void desenharJogador( Jogador *j ) {
    DrawRectangleRec( j->ret, j->cor );
}

void desenharVidaPlacar(Jogador *j){
    int tamanhoFonte = 50;

    const char *textoVida = TextFormat("Vida: %d", j->vida);
    const char *textoPontuacao = TextFormat("Pontuacao: %d", j->pontuacao);
    int t = MeasureText(textoPontuacao,tamanhoFonte);

    DrawText(textoPontuacao, GetScreenWidth()-t-10, 20, tamanhoFonte, j->cor);
    DrawText(textoVida, 20, 20, tamanhoFonte, j->cor);
}


