#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "Inimigo.h"
#include "Inventario.h"
#include "../utils/utils.h"

#define between(val, start, end) (start<=val && val<=end)
void mover_player(Player *player, DeltaDirecao delta_direcao, int lim_esquerda, int lim_direita, int lim_cima, int lim_baixo)
{
    int novo_x = player->posicao.x + delta_direcao.dx;
    int novo_y = player->posicao.y + delta_direcao.dy;

    if(between(novo_x,lim_esquerda,lim_direita)) player->posicao.x = novo_x;
    if(between(novo_y,lim_cima,lim_baixo)) player->posicao.y = novo_y;

}

DeltaDirecao get_delta_direcao(WINDOW* win)
{
    DeltaDirecao delta = {0, 0};
    int tecla;

    while((tecla=wgetch(win))!=ERR)
    {
        // y aumenta pra baixo, x para direita
        if(tecla == KEY_UP || tecla== 'w') delta.dy--;
        if(tecla == KEY_DOWN || tecla== 's') delta.dy++;
        if(tecla == KEY_LEFT || tecla== 'a') delta.dx--;
        if(tecla == KEY_RIGHT || tecla== 'd') delta.dx++;
    }
    // Evite acumulo excessivo no vetor de direção
    if (delta.dx >  1) delta.dx =  1;
    if (delta.dx < -1) delta.dx = -1;
    if (delta.dy >  1) delta.dy =  1;
    if (delta.dy < -1) delta.dy = -1;

    return delta;
}



bool atacar_inimigo(Player *player, Inimigo *inimigo)
{
    if (gerar_chance_de_evasao_do_inimigo() > 95)
        return false;

    inimigo_tomar_dano(inimigo, gerar_rolagem_dano() + player->inventario.arma.dano);

    return true;
}

void tomar_dano(Player *player, AtaqueInimigo *atack)
{
    player->vida -= atack->dano * (1.0f-(player->defesa/100.0f)); // se defesa for 5, -5% de dano
}

Player* criar_player(const char* nome, Genero genero)
{
    Player* player = malloc(sizeof(Player));
    if(player==NULL)
    {
        exit_with_error(Exception_AllocationError);
    }
    strncpy(player->nome,nome, sizeof(player->nome)-1);
    player->genero=genero;
    player->inventario=init_inventario();
    player->karma=0;
    player->NumeroAndar=0;
    player->level=1;
    player->vida=30;
    player->vida_max=30;
    player->defesa=0;

    player->defesa += player->inventario.armadura.capacete.defesa;
    player->defesa += player->inventario.armadura.peitoral.defesa;
    player->defesa += player->inventario.armadura.grevas.defesa;
    player->defesa += player->inventario.armadura.botas.defesa;

    return player;
}

void free_player(Player *player)
{
    free(player);

}