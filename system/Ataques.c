#include "../objects/Inimigo.h"
#include "../objects/Player.h"
#include "../utils/utils.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




//----------------------------
//       COLISÃO
//----------------------------
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
        //          X  
        //   X   X      
        case BULLET:
            return (abs(pl_x - atq_x) <= hitbox && abs(pl_y - atq_y) <= hitbox);

        //          XXXX
        //   XXXX
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

        //    fase de aviso: tick_vida < velocidade
        //   XXXXXXXXX  =>  |||||||||
        //   XXXXXXXXX      ||||||||| 
        case AREA:
            if (Ataque->tick_vida < Ataque->velocidade)
                return false; // ainda em aviso
            
            int largura = Ataque->vel_horizontal;
            int altura = Ataque->vel_vertical;
            return (pl_x >= atq_x && pl_x <= atq_x + largura &&
                        pl_y >= atq_y && pl_y <= atq_y + altura);
            
            break;
        //    fase de aviso: tick_vida < velocidade
        //      X     V
        //      X  => V
        //      X     V
        case LASER:
            if (Ataque->tick_vida < Ataque->velocidade)
                return false;
            if (Ataque->direcao == VERTICAL)
                return abs(pl_x - atq_x) <= hitbox;
            if (Ataque->direcao == HORIZONTAL)
                return abs(pl_y - atq_y) <= hitbox;
            break;
                                                //  |
                                                //  g
        case PAREDE:                            //  a
            if(Ataque->direcao==VERTICAL)       //  p
            {                                   //  |
                if(abs(pl_y-Ataque->y)<=hitbox) // está no gap
                    return false;
                if(Ataque->x==pl_x) // esta na mesma coluna
                    return true;
            }
            else if(Ataque->direcao==HORIZONTAL) //  ----gap---- 
            {
                if ( abs(pl_x-Ataque->x)<=hitbox) // está no gap
                    return false;
                if(pl_y==Ataque->y)
                    return true;
            }
        default:
            break;
    }
    return false;
}








//--------------------------
//    SPAWN
//--------------------------









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
            atq->x = rand() % (max_x - 2) + 1;
            atq->y = 1;
            break;
        }
        if (atq->direcao == HORIZONTAL)
        {
            atq->x = 1;
            atq->y = rand() % (max_y - 2) + 1;
            break;
        }
        break;

    case AREA:
        int largura = atq->vel_horizontal;
        int altura = atq->vel_vertical;
        atq->x = rand() % (max_x - 2 - largura) + 1; // - largura para garantir que nao saia da box
        atq->y = rand() % (max_y - 2 - altura) + 1;
        break;
    

    case PAREDE:
        // VERTICAL
        if (atq->sentido == ESQUERDA_DIREITA)
        {
            atq->vel_horizontal = 1;
            atq->vel_vertical = 0;
            atq->x = 1;
            atq->y = rand() % (max_y-2)+1;      //buraco na parede
            break;
        }
        if (atq->sentido == DIREITA_ESQUERDA)
        {
            atq->vel_horizontal = -1;
            atq->vel_vertical = 0;
            atq->x = max_x-2;
            atq->y = rand() % (max_y - 2) + 1;  // buraco na parede
            break;
        }
        // HORIZONTAL
        if (atq->sentido == CIMA_BAIXO)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = 1;
            atq->x = rand() % (max_x - 2) + 1; // buraco na parede
            atq->y = 1; 
            break;
        }
        if (atq->sentido == BAIXO_CIMA)
        {
            atq->vel_horizontal = 0;
            atq->vel_vertical = -1;
            atq->y = max_y-2;
            atq->x = rand() % (max_x - 2) + 1; // buraco na parede
            break;
        }
        break;

    default:
        break;
    }
}







//-----------------------------------------
//           Atualização
//-----------------------------------------






void atualizar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq)
{
    if (!atq->ativo)
        return;

    if (atq->tipo_ataque == LASER || atq->tipo_ataque == AREA)
    {
        atq->tick_vida++;
        if (atq->tick_vida >= atq->velocidade * 2)      // a quantidade de ticks vai estar relacionada a velocidade,
            atq->ativo = false;                             //similarmente abaixo, que quando >=atq->velocidade vai mover
        return;                                             // aqui >=atq->velocidade vai acabar a fase de aviso, e a fase de dano vai durar a mesma quantidade de ticks        
    }

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














//-----------------------------------------
//           Renderização
//-----------------------------------------

bool piscando_visivel(int tick, int intervalo)
{
    return (tick / intervalo) % 2 == 0;
}

void desenhar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq)
{
    int max_x = getmaxx(area_esquiva);
    int max_y = getmaxy(area_esquiva);

    bool em_aviso;
    switch (atq->tipo_ataque)
    {
        case LINHA:
            bool sprite_unitario = strlen(atq->ataque_sprite)<=4; //por causa dos caracteres unicode, se for so 1 caracter vai repetir até ter hitbox deles, ex: hitbox= 3, char=I => III
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
            int largura = atq->vel_horizontal;
            int altura = atq->vel_vertical;
            em_aviso = atq->tick_vida < atq->velocidade;
            if (em_aviso)
            {
                if (piscando_visivel(atq->tick_vida, 16)) //CONTORNANDO A AREA
                {
                    for (int x = atq->x; x <= (atq->x+largura) ; x++)
                    {
                        mvwprintw(area_esquiva, atq->y, x, atq->ataque_sprite);
                        mvwprintw(area_esquiva, atq->y + altura, x, atq->ataque_sprite);
                    }
                    for(int y=atq->y; y <= (atq->y+altura) ;y++ )
                    {
                        mvwprintw(area_esquiva, y, atq->x, atq->ataque_sprite);
                        mvwprintw(area_esquiva, y, atq->x+largura, atq->ataque_sprite);
                    }
                }
            }
            else //PREENCHE A ALTURA
            {
                wattron(area_esquiva, A_BOLD);

                for (int y = atq->y; y <= (atq->y+altura); y++)
                {
                    for(int x = atq->x; x<= (atq->x+largura) ; x++)
                    {
                        mvwprintw(area_esquiva, y, x, atq->ataque_sprite);
                    }
                }
                wattroff(area_esquiva, A_BOLD);
            }
            break;




        case LASER:
            em_aviso = atq->tick_vida < atq->velocidade;
            if (em_aviso)
            {   
                if(piscando_visivel(atq->tick_vida,16))
                {
                    if (atq->direcao == VERTICAL)
                        for (int y = 1; y < max_y - 1; y++)
                            mvwprintw(area_esquiva, y, atq->x, atq->ataque_sprite);
                    else //(atq->direcao == HORIZONTAL)
                        for (int x = 1; x < max_x - 1; x++)
                            mvwprintw(area_esquiva, atq->y, x, atq->ataque_sprite);
                }
            }
            else
            {
                wattron(area_esquiva, A_BOLD | COLOR_PAIR(COR_ATIVO));

                if(atq->direcao == VERTICAL)
                    for (int y = 1; y < max_y - 1; y++)
                        mvwprintw(area_esquiva, y, atq->x, atq->ataque_sprite);
                else //(atq->direcao == HORIZONTAL)
                    for (int x = 1; x < max_x - 1; x++)
                        mvwprintw(area_esquiva, atq->y, x, atq->ataque_sprite);
                wattroff(area_esquiva, A_BOLD | COLOR_PAIR(COR_ATIVO));
            }
            break;


        case BULLET: 
            wattron(area_esquiva, A_BOLD);
            mvwprintw(area_esquiva, atq->y, atq->x, "%s", atq->ataque_sprite);
            wattroff(area_esquiva, A_BOLD);
            break;

        case PAREDE:
            if(atq->direcao == HORIZONTAL)
            {
                for(int x=1;x<=max_x-2;x++)
                {
                    if (abs(x - atq->x) <= atq->hit_box)
                        continue;
                    mvwprintw(area_esquiva, atq->y, x, atq->ataque_sprite);
                }
            }
            else if(atq->direcao == VERTICAL)
            {
                for (int y = 1; y <= max_y - 2; y++)
                {
                    if (abs(y-atq->y) <= atq->hit_box)
                        continue;
                    mvwprintw(area_esquiva, y, atq->x, atq->ataque_sprite);
                }
            }
            break;



        default:
            break;
    }
}