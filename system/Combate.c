#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include "Combate.h"
#include "../objects/Inimigo.h"
#include "../objects/Player.h"
#include "../utils/utils.h"
#include <unistd.h>
#define equals(str1, str2) (strcmp(str1,str2)==0)
#define ataques_na_tela 32

void iniciar_combate(Player *player, Inimigo *inimigo)
{
    CombateUI ui;
    int max_x=getmaxx(stdscr);
    int esquiva_max_y, esquiva_max_x;


    // NOME BOSS
    ui.area_nome_boss = newwin(3, strlen(inimigo->nome) + 60 + inimigo->sprite_size.x, 0, 20);
    renderizar_nome_estilizado(ui.area_nome_boss,inimigo->nome);

    
    // BOSS
    ui.area_boss = newwin(inimigo->sprite_size.y + 5, inimigo->sprite_size.x + 2, 5, 0);
    desenhar_sprite(ui.area_boss, inimigo->sprite, 1, 1);



    // AREA ESQUIVA E PLAYER
    ui.area_esquiva = newwin(16,56, 6, inimigo->sprite_size.x+20);

    getmaxyx(ui.area_esquiva, esquiva_max_y, esquiva_max_x);
    player->posicao.x=esquiva_max_x/2;
    player->posicao.y=esquiva_max_y/2;


    // MENU
    ui.area_menu = newwin(20,56, 22,inimigo->sprite_size.x+20);
    renderizar_combate_ui(&ui, player);


    keypad(ui.area_esquiva, TRUE);
    wtimeout(ui.area_esquiva, 16);

    int frame=0;
    AtaqueInimigo ataques_ativos[ataques_na_tela];
    for(int i=0;i<ataques_na_tela;i++)
    {
        ataques_ativos[i].ativo = false;
    }


    while(inimigo->vida>0 && player->vida>0)
    {
        
        frame++;

        werase(ui.area_esquiva);
        box(ui.area_esquiva, 0, 0);
       // desenhar_ataque(ui.area_esquiva, &ataques_ativos[0]);
        //atualizar_ataque(ui.area_esquiva, &ataques_ativos[0]);
        //PMI if(frame%60==0) //1 ataques por cada 60 frames é muito?
        if(frame % 30 == 0)
        {
            for(int i=0;i<ataques_na_tela;i++)
            {
                if(!ataques_ativos[i].ativo)
                {
                    ataques_ativos[i] = inimigo->ataques[rand() % 2 + 1]; //testar so o bullet
                    spawnar_ataque(&ataques_ativos[i], ui.area_esquiva);

                    break;
                }
            }
        }

        for (int i = 0; i < ataques_na_tela; i++)
        {
            if (ataques_ativos[i].ativo)
            {
                desenhar_ataque(ui.area_esquiva, &ataques_ativos[i]);
                atualizar_ataque(ui.area_esquiva, &ataques_ativos[i]);
                //atualizar_ataque(ui.area_esquiva, &ataques_ativos[i], frame );
                //frame = 0; // não continuar aumento igual louco
                if (ataque_colidiu(player, &ataques_ativos[i]))
                {
                    //player->vida -= ataques_ativos[i].dano;
                    tomar_dano(player, &ataques_ativos[i]);
                    ataques_ativos[i].ativo = false;  // ataque some ao acertar
                    renderizar_combate_ui(&ui, player); // atualiza HP no menu
                }
            }
        }
        //mvwprintw(ui.area_esquiva, player->posicao.y +2, player->posicao.x - 5, "teste mano"); // debug
        //assert(strcmp("^", ataques_ativos[0].ataque_sprite) == 0);
        desenhar_jogador(ui.area_esquiva, player);
        mover_player(player, get_delta_direcao(ui.area_esquiva), 1, esquiva_max_x - 2, 1, esquiva_max_y - 2);
        wrefresh(ui.area_esquiva);
    }

}

bool ataque_colidiu(Player *player, AtaqueInimigo* Ataque)
{
    if(!Ataque->ativo)
        return false;
    int pl_x=player->posicao.x;
    int pl_y=player->posicao.y;
    int atq_x=Ataque->x;
    int atq_y=Ataque->y;
    int hitbox=Ataque->hit_box;

    switch (Ataque->tipo_ataque)
    {
        case BULLET:
            return ( abs(pl_x - atq_x)<=hitbox && abs(pl_y - atq_y)<=hitbox);
        case LINHA:
            if(Ataque->direcao == HORIZONTAL)
                return (abs(pl_y - atq_y)<=hitbox);
            if(Ataque->direcao == VERTICAL)
                return (abs(pl_x - atq_x) <= hitbox);
        case AREA:
            break;

        case ESFERA:
            break;

        case PAREDE:
            break;

        case LASER:
            if(Ataque->direcao == VERTICAL)
                return abs(pl_x - atq_x) <= hitbox;
            if(Ataque->direcao == HORIZONTAL)
                return abs(pl_y - atq_y) <= hitbox;
        default:
            break;
    }
    return false;
}
void spawnar_ataque(AtaqueInimigo *atq, WINDOW *area_esquiva)
{
    int max_x, max_y;
    getmaxyx(area_esquiva, max_y,max_x);
    atq->ativo = true;
    atq->tick_movimento = 0;  // ← zera aqui
    atq->tick_vida = 0;       // ← e aqui

    switch(atq->tipo_ataque)
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
            if(atq->sentido == DIREITA_ESQUERDA)
            {
                atq->vel_horizontal=-1;
                atq->vel_vertical=0;
                atq->x = max_x - 2;
                atq->y = rand() % (max_y-2)+1;
                break;
            }
            if(atq->sentido == CIMA_BAIXO)
            {
                atq->vel_horizontal=0;
                atq->vel_vertical=1;
                atq->x = rand() % (max_x-2)+1;
                atq->y = 1;
                break;
            }
            if(atq->sentido == BAIXO_CIMA)
            {
                atq->vel_horizontal=0;
                atq->vel_vertical=-1;
                atq->x = rand() % (max_x-2)+1;
                atq->y = max_y-2;
                break;
            }
            break;
        case LINHA:
            // cobre largura toda, desce devagar
            if(atq->direcao == VERTICAL)
            {
                atq->vel_horizontal = 0;
                atq->vel_vertical = 1;
                atq->x = 1;
                atq->y = 1;
                break;
            }
            if(atq->sentido == ESQUERDA_DIREITA)
            {
                atq->vel_horizontal = 1;
                atq->vel_vertical = 0;
                atq->x = 1;
                atq->y = rand()%(max_y-2);             
                break;
            }
            if(atq->sentido == DIREITA_ESQUERDA)
            {
                atq->vel_horizontal = 1;
                atq->vel_vertical = 0;
                atq->x = max_x-2;
                atq->y = rand()%(max_y-2);             
                break;
            }
            break;
        case LASER:
            if(atq->direcao == VERTICAL)
            {
                atq->vel_horizontal = 0;
                atq->vel_vertical = 0;
                atq->x = rand() % (max_x - 2) + 1;
                atq->y = 1;
                break;
            }
            if(atq->direcao == HORIZONTAL)
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


//PMI
void atualizar_ataque(WINDOW *area_esquiva, AtaqueInimigo *atq)
{
    if (!atq->ativo) return;

    atq->tick_movimento++;

    if (atq->tick_movimento < atq->velocidade)
        return;

    atq->tick_movimento = 0;

    atq->x += atq->vel_horizontal;
    atq->y += atq->vel_vertical;

    if (0 >= atq->x || atq->x >= getmaxx(area_esquiva)-1 ||
        0 >= atq->y || atq->y >= getmaxy(area_esquiva)-1)
    {
        atq->ativo = false;
    }
}

void desenhar_ataque(WINDOW* area_esquiva, AtaqueInimigo* atq)
{
    int max_x = getmaxx(area_esquiva);
    switch (atq->tipo_ataque)
    {
        case LINHA:
            // desenha da borda esquerda até a direita na linha y, tem que ajeitar isso
            //if(atq->direcao == VERTICAL)

            //if(atq->direcao == HORIZONTAL)
                for (int x = 1; x < max_x - 1; x++)
                    mvwprintw(area_esquiva, atq->y, x, "%s", atq->ataque_sprite);
                break;

        default:
            mvwprintw(area_esquiva, atq->y, atq->x, "%s", atq->ataque_sprite);
            break;
    }

}


bool rodada(/*AtaqueInimigo ataque, Player* player*/)
{
    EstagioCombate estado_atual = FASE_DIALOGO1;

    // fala do boss
    // 1 sequencia de um ataque especifico
    // ação do player
    // fala do boss
}




void desenhar_botao(WINDOW *area_menu, const char *texto, int y, int x, bool selecionada)
{
    if (selecionada)
        wattron(area_menu, (COR_OPCAO_SELECIONADA) | A_BOLD);

    mvwprintw(area_menu, y, x, "+--------------+");
    mvwprintw(area_menu, y + 1, x, "|  %-12s|", texto);
    mvwprintw(area_menu, y + 2, x, "+--------------+");

    if (selecionada)
        wattroff(area_menu, (COR_OPCAO_SELECIONADA) | A_BOLD);
}

void renderizar_menu_combate(WINDOW *area_menu, Player* player, OpcoesMenuCombate opcao_hovered)
{
    werase(area_menu);

    mvwprintw(area_menu, 1, 3, "HP:%d/%d", player->vida, player->vida_max);

    // Desenha a barra de vida
    mvwprintw(area_menu,1,15,"[");
    int porcentagem_vida=(player->vida*10/player->vida_max);
    wattron(area_menu, COLOR_PAIR(COR_VIDA));
    for (int x = 16, i = 0; i < porcentagem_vida; i++, x++)
        mvwprintw(area_menu, 1, x, "█");
    wattroff(area_menu, COLOR_PAIR(COR_VIDA));
    mvwprintw(area_menu, 1, 26, "]");

    mvwprintw(area_menu,1, 50, "Lv.%d", player->level);


    desenhar_botao(area_menu, "1.ATACAR", 3, 4, opcao_hovered == ATACAR);
    desenhar_botao(area_menu, "2.ITENS", 3, 37, opcao_hovered == ITENS);
    desenhar_botao(area_menu, "3.MERCY", 6, 4, opcao_hovered == MERCY);
    desenhar_botao(area_menu, "4.DESISTIR", 6, 37, opcao_hovered == DESISTIR);
}

void renderizar_combate_ui(CombateUI* ui, Player* player)
{
    box(ui->area_boss,0,0);
    box(ui->area_esquiva,0,0);
    desenhar_jogador(ui->area_esquiva, player);
    renderizar_menu_combate(ui->area_menu, player, INVALIDA);

    wrefresh(ui->area_nome_boss);
    wrefresh(ui->area_boss);
    wrefresh(ui->area_esquiva);
    wrefresh(ui->area_menu);
}






void desenhar_jogador(WINDOW *area_esquiva, Player *player)
{
    mvwprintw(area_esquiva, player->posicao.y, player->posicao.x, "🤍");
}



void renderizar_nome_estilizado(WINDOW* area_nome_boss, const char* nome)
{
    int meio = (getmaxx(area_nome_boss) / 2) - 10 - strlen(nome);
    wattron(area_nome_boss, COLOR_PAIR(COR_NOME_BOSS) | A_BOLD);

    if(equals("Fallen King",nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "██████ ▄▄▄  ▄▄    ▄▄    ▄▄▄▄▄ ▄▄  ▄▄   ██ ▄█▀ ▄▄ ▄▄  ▄▄  ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██▄▄  ██▀██ ██    ██    ██▄▄  ███▄██   ████   ██ ███▄██ ██ ▄▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "██    ██▀██ ██▄▄▄ ██▄▄▄ ██▄▄▄ ██ ▀██   ██ ▀█▄ ██ ██ ▀██ ▀███▀", nome);
    }
    if (equals("Hollow Knight", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "██  ██  ▄▄▄  ▄▄    ▄▄     ▄▄▄  ▄▄   ▄▄   ██ ▄█▀ ▄▄  ▄▄ ▄▄  ▄▄▄▄ ▄▄ ▄▄ ▄▄▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██████ ██▀██ ██    ██    ██▀██ ██ ▄ ██   ████   ███▄██ ██ ██ ▄▄ ██▄██   ██   ", nome);
        mvwprintw(area_nome_boss, 2, meio, "██  ██ ▀███▀ ██▄▄▄ ██▄▄▄ ▀███▀  ▀█▀█▀    ██ ▀█▄ ██ ▀██ ██ ▀███▀ ██ ██   ██   ", nome);
    }
    if (equals("Cerberus", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "▄█████ ▄▄▄▄▄ ▄▄▄▄  ▄▄▄▄  ▄▄▄▄▄ ▄▄▄▄  ▄▄ ▄▄  ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██     ██▄▄  ██▄█▄ ██▄██ ██▄▄  ██▄█▄ ██ ██ ███▄▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "▀█████ ██▄▄▄ ██ ██ ██▄█▀ ██▄▄▄ ██ ██ ▀███▀ ▄▄██▀ ", nome);
    }
    if (equals("Demon", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "████▄  ▄▄▄▄▄ ▄▄   ▄▄  ▄▄▄  ▄▄  ▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██  ██ ██▄▄  ██▀▄▀██ ██▀██ ███▄██", nome);
        mvwprintw(area_nome_boss, 2, meio, "████▀  ██▄▄▄ ██   ██ ▀███▀ ██ ▀██", nome);
    }
    if (equals("Dragon", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "████▄  ▄▄▄▄   ▄▄▄   ▄▄▄▄  ▄▄▄  ▄▄  ▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██  ██ ██▄█▄ ██▀██ ██ ▄▄ ██▀██ ███▄██", nome);
        mvwprintw(area_nome_boss, 2, meio, "████▀  ██ ██ ██▀██ ▀███▀ ▀███▀ ██ ▀██", nome);
    }
    if (equals("Centaur", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "▄█████ ▄▄▄▄▄ ▄▄  ▄▄ ▄▄▄▄▄▄ ▄▄▄  ▄▄ ▄▄ ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██     ██▄▄  ███▄██   ██  ██▀██ ██ ██ ██▄█▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "▀█████ ██▄▄▄ ██ ▀██   ██  ██▀██ ▀███▀ ██ ██", nome);
    }
    wattroff(area_nome_boss, COLOR_PAIR(COR_NOME_BOSS) | A_BOLD);
}


void limpar_combate(CombateUI *ui, Inimigo *inimigo);