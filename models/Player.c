#include "Player.h"
#include "Inimigo.h"

#include "../utils/utils.h"

void move(Player *player, Direcao direcao /*, int limite_esquerda, int limite_direita,int limite_cima, int limite_baixo*/)
{
    switch(direcao)
    {
        case ESQUERDA:
            // if(player->posicao.x-1>limite_esquerda)
            player->posicao.x--;
            break;
        case DIREITA:
            // if(player->posicao.x+1<limite_direita)
            player->posicao.x++;
            break;
        case BAIXO:
            // if(player->posicao.y-1>limite_baixo)
            player->posicao.y--;
            break;
        case CIMA:
            // if(player->posicao.y+1<limite_cima)
            player->posicao.y++;

            break;
    }
}
bool atacar_inimigo(Player *player, Inimigo *inimigo)
{
    if (gerar_chance_de_evasao_do_inimigo() > 95)
        return false;

    inimigo_tomar_dano(inimigo, gerar_rolagem_dano() + player->inventario.arma->dano);

    return true;
}