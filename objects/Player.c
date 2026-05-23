#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "Inimigo.h"
#include "Inventario.h"
#include "../utils/utils.h"
#include "Atributos.h"



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

    inimigo_tomar_dano(inimigo, gerar_rolagem_dano() + player->inventario.arma.dano);

    return true;
}

void tomar_dano(Player *player, AtaqueInimigo *atack)
{
    player->vida -= atack->dano * (1.0f-(player->atributos.defesa/100.0f)); // se defesa for 5, -5% de dano
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
    player->vida = 20;
    //ATRIBUTOS BASE
    player->atributos.defesa= 5;
    player->atributos.forca= 5;
    player->atributos.sorte= 5;

    // PEGA O ATRIBUTO BASE DO PLAYER, E SOMA COM OS ATRIBUTOS DA ARMADURA E ARMA QUE O PLAYER ESTA EQUIPADO
    player->vida += player->inventario.armadura.capacete.vida;
    player->vida += player->inventario.armadura.peitoral.vida;
    player->vida += player->inventario.armadura.grevas.vida;
    player->vida += player->inventario.armadura.botas.vida;

    player->vida += player->inventario.arma.vida;
    player->atributos.forca += player->inventario.arma.atributosarma.forca;
    player->atributos.defesa += player->inventario.arma.atributosarma.defesa;
    player->atributos.sorte += player->inventario.arma.atributosarma.sorte;

    player->atributos.defesa += player->inventario.armadura.capacete.atributosarmadura.defesa;
    player->atributos.defesa += player->inventario.armadura.peitoral.atributosarmadura.defesa;
    player->atributos.defesa += player->inventario.armadura.grevas.atributosarmadura.defesa;
    player->atributos.defesa += player->inventario.armadura.botas.atributosarmadura.defesa;

    player->atributos.forca += player->inventario.armadura.capacete.atributosarmadura.forca;
    player->atributos.forca += player->inventario.armadura.peitoral.atributosarmadura.forca;
    player->atributos.forca += player->inventario.armadura.grevas.atributosarmadura.forca;
    player->atributos.forca += player->inventario.armadura.botas.atributosarmadura.forca;

    player->atributos.sorte += player->inventario.armadura.capacete.atributosarmadura.sorte;
    player->atributos.sorte += player->inventario.armadura.peitoral.atributosarmadura.sorte;
    player->atributos.sorte += player->inventario.armadura.grevas.atributosarmadura.sorte;
    player->atributos.sorte += player->inventario.armadura.botas.atributosarmadura.sorte;

    return player;
}

void free_player(Player *player)
{
    free(player);

}