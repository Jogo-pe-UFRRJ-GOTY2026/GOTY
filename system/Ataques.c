#include "../objects/Inimigo.h"
#include "../objects/Player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


bool ataque_colidiu(Player *player, AtaqueInimigo *Ataque)
{
    if (!Ataque->ativo)
        return false;
    int pl_x = player->posicao.x;
    int pl_y = player->posicao.y;
    int atq_x = Ataque->x;
    int atq_y = Ataque->y;
    int hitbox = Ataque->hit_box;

    switch (Ataque->tipo_ataque)
    {
    case BULLET:
        return (abs(pl_x - atq_x) <= hitbox && abs(pl_y - atq_y) <= hitbox);
    case LINHA:
        if (Ataque->direcao == HORIZONTAL)
            return  pl_y == atq_y &&
                    pl_x >= atq_x &&
                    pl_x <= atq_x + Ataque->hit_box;
        if (Ataque->direcao == VERTICAL)
        
            return  pl_x == atq_x &&
                    pl_y >= atq_y &&
                    pl_y <= atq_y + Ataque->hit_box;
            break;
    case AREA:
        break;

    case ESFERA:
        //int ddx = pl_x - atq_x;
        //int ddy = pl_y - atq_y;
        //return (ddx * ddx + ddy * ddy) <= (atq->hit_box * atq->hit_box);
        break;

    case PAREDE:
        break;

    case LASER:
        if (Ataque->direcao == VERTICAL)
            return abs(pl_x - atq_x) <= hitbox;
        if (Ataque->direcao == HORIZONTAL)
            return abs(pl_y - atq_y) <= hitbox;
    default:
        break;
    }
    return false;
}

void spawnar_ataque(AtaqueInimigo *atq, WINDOW *area_esquiva)
{
    int max_x, max_y;
    getmaxyx(area_esquiva, max_y, max_x);
    atq->ativo = true;
    atq->tick_movimento = 0; // ← zera aqui
    atq->tick_vida = 0;              // ← e aqui

    switch (atq->tipo_ataque)
    {
    case BULLET:

        if (atq->sentido == ESQUERDA_DIREITA)
        {
            atq->vel_horizontal = 1;
            atq->vel_vertical = 0;
            atq->y = rand() % (max_y - 2) + 1;
            atq->x = 2;
            break;
        }
        if (atq->sentido == DIREITA_ESQUERDA)
        {
            atq->vel_horizontal = -1;
            atq->vel_vertical = 0;
            atq->x = max_x - 2;
            atq->y = rand() % (max_y - 2) + 1;
            break;
        }
        if (atq->sentido == CIMA_BAIXO)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = 1;
            atq->x = rand() % (max_x - 2) + 1;
            atq->y = 1;
            break;
        }
        if (atq->sentido == BAIXO_CIMA)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = -1;
            atq->x = rand() % (max_x - 2) + 1;
            atq->y = max_y - 2;
            break;
        }
        break;


    case LINHA:
        // A ideia é ser tipo um beam
        if (atq->sentido == ESQUERDA_DIREITA)
        {
            atq->vel_horizontal = 1;
            atq->vel_vertical = 0;
            atq->x = 2;
            atq->y = rand() % (max_y - 2);
            break;
        }
        if (atq->sentido == DIREITA_ESQUERDA)
        {
            atq->vel_horizontal = -1;
            atq->vel_vertical = 0;
            atq->x = max_x - 2;
            atq->y = rand() % (max_y - 2);
            break;
        }
        break;


    case LASER:
        //vai ter que usar os ticks de vida do ataque nesse tipo como 
        if (atq->direcao == VERTICAL)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = 0;
            atq->x = rand() % (max_x - 2) + 1;
            atq->y = 1;
            break;
        }
        if (atq->direcao == HORIZONTAL)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = 0;
            atq->x = 1;
            atq->y = rand() % (max_y - 2) + 1;
            break;
        }
        break;
    }
}

void atualizar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq)
{
    if (!atq->ativo)
        return;

    atq->tick_movimento++;

    if (atq->tick_movimento < atq->velocidade)
        return;

    atq->tick_movimento = 0;

    atq->x += atq->vel_horizontal;
    atq->y += atq->vel_vertical;

    if (0 >= atq->x || atq->x >= getmaxx(area_esquiva) - 1 ||
        0 >= atq->y || atq->y >= getmaxy(area_esquiva) - 1)
    {
        atq->ativo = false;
    }
}

void desenhar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq)
{
    int max_x = getmaxx(area_esquiva);
    switch (atq->tipo_ataque)
    {
        case LINHA:
        // A ideia é como se fosse um BEAM , com os caracteres do sprite dela
            bool sprite_unitario = strlen(atq->ataque_sprite)<=4;
            if(atq->direcao == HORIZONTAL)
            {
                
                if(sprite_unitario)
                {
                    for (int i = 0; i < atq->hit_box && atq->sentido==ESQUERDA_DIREITA; i++)
                        mvwprintw(area_esquiva, atq->y, atq->x + i, "%s", atq->ataque_sprite);

                    for (int i = 0; i < atq->hit_box && atq->sentido==DIREITA_ESQUERDA; i++)
                            mvwprintw(area_esquiva, atq->y, atq->x - i, "%s", atq->ataque_sprite);
                    
                }
                else
                    mvwprintw(area_esquiva, atq->y, atq->x, "%s", atq->ataque_sprite);
                break;
            }
            if (atq->direcao == VERTICAL)
            {
                if (sprite_unitario)
                {
                    for (int i = 0; i < atq->hit_box && atq->sentido == CIMA_BAIXO; i++)
                        mvwprintw(area_esquiva, atq->y + i, atq->x, "%s", atq->ataque_sprite);

                    for (int i = 0; i < atq->hit_box && atq->sentido == BAIXO_CIMA; i++)
                        mvwprintw(area_esquiva, atq->y -i, atq->x, "%s", atq->ataque_sprite);
                }
                else
                    mvwprintw(area_esquiva, atq->y, atq->x, "%s", atq->ataque_sprite);
                break;
            }
            break;

        case AREA: // Retangulo
        case ESFERA:
        case PAREDE:
        case LASER:
            break;
        case BULLET: 
            wattron(area_esquiva, A_BOLD);
            mvwprintw(area_esquiva, atq->y, atq->x, "%s", atq->ataque_sprite);
            wattroff(area_esquiva, A_BOLD);
            break;
        default:
            break;
    }
}