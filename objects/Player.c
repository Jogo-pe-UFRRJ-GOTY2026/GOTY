#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "Inimigo.h"
#include "Inventario.h"
#include "../utils/utils.h"



void mover_player(Player *player, Direcao direcao, int limite_esquerda, int limite_direita,int limite_cima, int limite_baixo)
{
    switch(direcao)
    {
        case ESQUERDA:
            if(player->posicao.x-1>limite_esquerda)
                player->posicao.x--;
            break;
        case DIREITA:
            if(player->posicao.x+1<limite_direita)
                player->posicao.x++;
            break;
        case BAIXO:
            if(player->posicao.y-1>limite_baixo)
                player->posicao.y--;
            break;
        case CIMA:
            if(player->posicao.y+1<limite_cima)
                player->posicao.y++;
            break;
        default:
            break;
    }
}

Direcao get_direcao(char tecla_pressionada)
{
    if(tecla_pressionada == KEY_UP    || tecla_pressionada=='w') 
        return CIMA;
    else if(tecla_pressionada == KEY_DOWN  || tecla_pressionada=='s') 
        return BAIXO;
    else if(tecla_pressionada == KEY_LEFT  || tecla_pressionada=='a') 
        return ESQUERDA;
    else if(tecla_pressionada == KEY_RIGHT || tecla_pressionada=='d') 
        return DIREITA;
    return DIRECAO_INVALIDA;
}



bool atacar_inimigo(Player *player, Inimigo *inimigo)
{
    if (gerar_chance_de_evasao_do_inimigo() > 95)
        return false;

    inimigo_tomar_dano(inimigo, gerar_rolagem_dano() + player->inventario->arma->dano);

    return true;
}

void tomar_dano(Player *player, AtaqueInimigo *atack)
{
    //player->vida -= seila;
}

Player* criar_player(char nome[100], Genero genero)
{
    Player* player = malloc(sizeof(Player));
    strcpy(player->nome,nome);
    player->genero=genero;
    player->inventario=init_inventario();
    player->karma=0;
    player->NumeroAndar=0;
    player->level=1;
    player->defesa=0;

    player->defesa += player->inventario->armadura->capacete.defesa;
    player->defesa += player->inventario->armadura->peitoral.defesa;
    player->defesa += player->inventario->armadura->grevas.defesa;
    player->defesa += player->inventario->armadura->botas.defesa;

    return player;
}
