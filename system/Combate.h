#ifndef COMBATE_H
#define COMBATE_H

typedef enum Estagio_atual_combate{
    FASE_DIALOGO,
    FASE_ESQUIVA,
    FASE_ACAO_JOGADOR,
    VITORIA,
    DERROTA
} EstagioCombate;

void init_combat();

#endif