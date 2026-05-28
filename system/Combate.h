#ifndef COMBATE_H
#define COMBATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "../objects/Inimigo.h"
#include "../objects/Player.h" 

typedef struct CombateUI{
    WINDOW* area_boss;
    WINDOW* area_esquiva;
    WINDOW* area_menu;
    WINDOW* area_nome_boss;
    WINDOW* area_dialogos;
    WINDOW* area_vida_boss;
} CombateUI;

typedef enum OpcoesMenuCombate
{
    ATACAR=1,
    ITENS,
    MERCY,
    DESISTIR,
    INVALIDA
} OpcoesMenuCombate ;

typedef enum EstadoRodada_
{
    FASE_DIALOGO1,
    FASE_ESQUIVA,
    FASE_ACAO_JOGADOR,
    FASE_DIALOGO2,
    VITORIA,
    DERROTA,
    CONTINUA
} EstadoRodada;

EstadoRodada iniciar_combate(Player *player, Inimigo *inimigo);
EstadoRodada rodada(AtaqueInimigo ataque, Player *player, Inimigo *inimigo, CombateUI *ui);
EstadoRodada loop_esquiva(AtaqueInimigo ataque_atual, Player *player, CombateUI *ui, int tempo_rodada);


bool ataque_colidiu(Player *player, AtaqueInimigo *Ataque);
void spawnar_ataque(AtaqueInimigo *Ataque, WINDOW *area_esquiva);
void atualizar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq);


void renderizar_menu_combate(WINDOW *area_menu, Player *player, OpcoesMenuCombate opcao_hovered);
void renderizar_combate_ui(CombateUI *ui, Player *player, Inimigo* inimigo);
void renderizar_vida_boss(WINDOW *area_vida_boss, Inimigo *inimigo);
void desenhar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq);
void desenhar_jogador(WINDOW *area_esquiva, Player *player);


void limpar_combate(CombateUI *ui, Inimigo *inimigo);
void renderizar_nome_estilizado(WINDOW *area_nome_boss, const char *nome);

#endif