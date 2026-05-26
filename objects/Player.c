#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "Inimigo.h"
#include "Inventario.h"
#include "../utils/utils.h"
#include "Atributos.h"

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

void tomar_dano(Player *player, AtaqueInimigo *attack)
{
    int defesaTotal = defesa_total(player);

    float reducao = defesaTotal / 100.0f;

    if(reducao > 0.80f)
        reducao = 0.80f;

    int danoFinal = attack->dano * (1.0f - reducao);

    if(danoFinal < 1)
        danoFinal = 1;

    player->vida -= danoFinal;

    if(player->vida < 0)
        player->vida = 0;
}

int vida_max_total(Player *player)
{
    int vida = player->vida_max;

    vida += player->inventario.arma.vida;

    vida += player->inventario.armadura.capacete.vida;
    vida += player->inventario.armadura.peitoral.vida;
    vida += player->inventario.armadura.grevas.vida;
    vida += player->inventario.armadura.botas.vida;

    return vida;
}

int defesa_total(Player *player)
{
    int defesa = player->atributos.defesa;

    defesa += player->inventario.arma.atributosarma.defesa;

    defesa += player->inventario.armadura.capacete.atributosarmadura.defesa;
    defesa += player->inventario.armadura.peitoral.atributosarmadura.defesa;
    defesa += player->inventario.armadura.grevas.atributosarmadura.defesa;
    defesa += player->inventario.armadura.botas.atributosarmadura.defesa;

    return defesa;
}

int forca_total(Player *player)
{
    int forca = player->atributos.forca;

    forca += player->inventario.arma.atributosarma.forca;

    forca += player->inventario.armadura.capacete.atributosarmadura.forca;
    forca += player->inventario.armadura.peitoral.atributosarmadura.forca;
    forca += player->inventario.armadura.grevas.atributosarmadura.forca;
    forca += player->inventario.armadura.botas.atributosarmadura.forca;

    return forca;
}

int sorte_total(Player *player)
{
    int sorte = player->atributos.sorte;

    sorte += player->inventario.arma.atributosarma.sorte;

    sorte += player->inventario.armadura.capacete.atributosarmadura.sorte;
    sorte += player->inventario.armadura.peitoral.atributosarmadura.sorte;
    sorte += player->inventario.armadura.grevas.atributosarmadura.sorte;
    sorte += player->inventario.armadura.botas.atributosarmadura.sorte;

    return sorte;
}

Player* criar_player(const char* nome, Genero genero)
{
    Player* player = malloc(sizeof(Player));
    if(player == NULL)
    {
        exit_with_error(Exception_AllocationError);
    }

    strncpy(player->nome, nome, sizeof(player->nome) - 1);

    player->nome[sizeof(player->nome) - 1] = '\0';
    player->genero = genero;
    player->inventario = init_inventario();
    player->karma = 0;
    player->NumeroAndar = 0;
    player->level = 1;

    // ATRIBUTOS BASE

    player->vida_max = 20;
    player->atributos.defesa = 5;
    player->atributos.forca = 5;
    player->atributos.sorte = 5;

    // VIDA FINAL COM EQUIPAMENTOS

    player->vida = vida_max_total(player);

    // ZERA O MEDIDOR DE LEMBRANÇAS

    for(int i = 0; i < 8; i++)
    {
        player->medidor_lembranca[i] = 0;
    }

    return player;
}

void free_player(Player *player)
{
    free(player);

}