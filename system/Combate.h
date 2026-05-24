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
} CombateUI;

typedef enum OpcoesMenuCombate
{
    ATACAR,
    ITENS,
    MERCY,
    DESISTIR,
    INVALIDA
} OpcoesMenuCombate ;

typedef enum Estagio_atual_combate
{
    FASE_DIALOGO1,
    FASE_ESQUIVA,
    FASE_DIALOGO2,
    FASE_ACAO_JOGADOR,
    FASE_DIALOGO3,
} EstagioCombate;

void desenhar_jogador(WINDOW *area_esquiva, Player *player);
void iniciar_combate(Player *player,Inimigo* inimigo);
bool ataque_colidiu(Player *player, AtaqueInimigo* Ataque);
void spawnar_ataque(AtaqueInimigo *Ataque, WINDOW *area_esquiva);
void renderizar_combate_ui(CombateUI* ui, Player* player);
void atualizar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq, int frame_atual);
void desenhar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq);
void limpar_combate(CombateUI *ui, Inimigo *inimigo);
void renderizar_nome_estilizado(WINDOW *area_nome_boss, const char *nome);
bool rodada();

#endif