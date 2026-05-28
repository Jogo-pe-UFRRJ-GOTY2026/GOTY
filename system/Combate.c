#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "Combate.h"
#include "../objects/Inimigo.h"
#include "../objects/Player.h"
#include "../utils/utils.h"
#define ataques_na_tela 32

EstadoRodada iniciar_combate(Player *player, Inimigo *inimigo)
{
    CombateUI ui;
    int esquiva_max_y, esquiva_max_x;


    // NOME BOSS
    ui.area_nome_boss = newwin(3, strlen(inimigo->nome) + 60 + inimigo->sprite_size.x, 0, 15);
    renderizar_nome_estilizado(ui.area_nome_boss,inimigo->nome);

    
    // BOSS
    ui.area_boss = newwin(inimigo->sprite_size.y + 5, inimigo->sprite_size.x + 2, 5, 5);
    desenhar_sprite(ui.area_boss, inimigo->sprite, 1, 1);

    //VIDA BOSS
    ui.area_vida_boss = newwin(3,40,5,inimigo->sprite_size.x+30);

    const int LARGURA_UI = 40;
    // AREA ESQUIVA E PLAYER - acaba em y = 18
    ui.area_esquiva = newwin(12,40, 6, inimigo->sprite_size.x+28);

    getmaxyx(ui.area_esquiva, esquiva_max_y, esquiva_max_x);
    player->posicao.x=esquiva_max_x/2;
    player->posicao.y=esquiva_max_y/2;

    // AREA DE DIALOGOS
    ui.area_dialogos = newwin(3, 60, 19, inimigo->sprite_size.x + 20);

    // MENU
    ui.area_menu = newwin(25,40, 24,inimigo->sprite_size.x+28);
    renderizar_combate_ui(&ui, player, inimigo);


    keypad(ui.area_esquiva, TRUE);
    wtimeout(ui.area_esquiva, 16);

    EstadoRodada resultado_combate;

    while (inimigo->vida > 0 && player->vida > 0)
    {
        resultado_combate = rodada(inimigo->ataques[rand() % inimigo->numero_ataques], player, inimigo, &ui);
        if (resultado_combate == VITORIA || resultado_combate == DERROTA)
            break;
    }

    return resultado_combate; //Tratar das falas especificas com base no retorno e do hp do jogador
}

EstadoRodada rodada(AtaqueInimigo ataque, Player *player,Inimigo* inimigo, CombateUI* ui)
{
    EstadoRodada atual;


    // ------------------------------------ DIALOGO 1


    

    atual = FASE_ESQUIVA;
    EstadoRodada resultado_esquiva = loop_esquiva(ataque, player, ui, inimigo->tempo_por_rodada);
    if(resultado_esquiva==DERROTA)
        return DERROTA;



    // ---------------- ACAO JOGADOR ----------------

    atual = FASE_ACAO_JOGADOR;

    //ação do player


    OpcoesMenuCombate opcao_atual = ATACAR;
    int tecla;
    renderizar_menu_combate(ui->area_menu, player, opcao_atual);

    while(tecla = wgetch(ui->area_menu))
    {
        if(tecla == KEY_ENTER || tecla==10)
            break;
        switch (tecla)
        {
            case '1': 
                opcao_atual=ATACAR;
                break;
            case '2':
                opcao_atual = ITENS;
                break;
            case '3':
                opcao_atual = MERCY;

                break;
            case '4':
                opcao_atual = DESISTIR;
                
                break;
            default:
                break;
        }
        renderizar_menu_combate(ui->area_menu, player, opcao_atual);

    }
    OpcoesMenuCombate opcao_escolhida = opcao_atual;
    switch (opcao_escolhida)
    {
        case ATACAR:
            werase(ui->area_dialogos);
            box(ui->area_dialogos, 0, 0);
            slow_mvwprintw(ui->area_dialogos, inimigo->dialogos_ataque[rand() % 5], 1, 2, 40);
            napms(1000);
            werase(ui->area_dialogos);
            wrefresh(ui->area_dialogos);
            renderizar_vida_boss(ui->area_vida_boss, inimigo);
            wrefresh(ui->area_vida_boss);
            atacar_inimigo(player, inimigo);
            break;
        case ITENS:
            break;
        case MERCY:
            werase(ui->area_dialogos);
            box(ui->area_dialogos, 0, 0);
            slow_mvwprintw(ui->area_dialogos, inimigo->dialogos_mercy[rand() % 2], 1, 2, 40);
            napms(1000);
            werase(ui->area_dialogos);
            wrefresh(ui->area_dialogos);

            if(inimigo->mercy>=100)
            {
                return VITORIA;
            }
            break;
        case DESISTIR:
            return DERROTA;
    }

    if (inimigo->vida <= 0)
    {
        return VITORIA;
    }
    // ---------------- DIALOGO AO FIM DA RODADA ----------------

    atual = FASE_DIALOGO2;


    return CONTINUA;
}

//RENDERIZAR COISAS

EstadoRodada loop_esquiva(AtaqueInimigo ataque_atual, Player *player, CombateUI *ui, int tempo_rodada)
{
    bool ha_ataques_na_area=true;
    bool spawn_ataques_habilitado=true;
    AtaqueInimigo ataques_ativos[ataques_na_tela];
    for (int i = 0; i < ataques_na_tela; i++)
    {
        ataques_ativos[i].ativo = false;
    }
    time_t inicio_rodada_esquiva = time(NULL);
    int esquiva_max_y, esquiva_max_x;
    getmaxyx(ui->area_esquiva, esquiva_max_y, esquiva_max_x);

    int frame=0;
    while (ha_ataques_na_area || spawn_ataques_habilitado)
    {
        if(! (difftime(time(NULL), inicio_rodada_esquiva) <= tempo_rodada))
            spawn_ataques_habilitado=false;

        frame++;
        werase(ui->area_esquiva);
        box(ui->area_esquiva, 0, 0);

        // spawn dos tey twey shwiin,
        if (spawn_ataques_habilitado && frame % 30 == 0)
        {
            for (int i = 0; i < ataques_na_tela; i++)
            {
                if (!ataques_ativos[i].ativo)
                {
                    ataques_ativos[i] = ataque_atual;
                    spawnar_ataque(&ataques_ativos[i], ui->area_esquiva);

                    break;
                }
            }
            frame = 0;
        }
        ha_ataques_na_area=false;
        // --- atualização dos ataaawques
        for (int i = 0; i < ataques_na_tela; i++)
        {
            if (ataques_ativos[i].ativo)
            {
                ha_ataques_na_area = true;
                desenhar_ataque(ui->area_esquiva, &ataques_ativos[i]);
                atualizar_ataque(ui->area_esquiva, &ataques_ativos[i]);
                // atualizar_ataque(ui.area_esquiva, &ataques_ativos[i], frame );
                // frame = 0; // não continuar aumento igual louco
                if (ataque_colidiu(player, &ataques_ativos[i]))
                {
                    tomar_dano(player, &ataques_ativos[i]);
                    ataques_ativos[i].ativo = false;    // ataque some ao acertar
                    renderizar_combate_ui(ui, player, NULL); // atualiza HP no menu
                }
            }
        }
        if(player->vida<=0) return DERROTA;
        desenhar_jogador(ui->area_esquiva, player);
        mover_player(player, get_delta_direcao(ui->area_esquiva), 1, esquiva_max_x - 2, 1, esquiva_max_y - 2);
        wrefresh(ui->area_esquiva);
    }
    return CONTINUA;
}

void desenhar_botao(WINDOW *area_menu, const char *texto, int y, int x, bool selecionada)
{
    if (selecionada)
    {
        wattron(area_menu, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);
        mvwprintw(area_menu,y+1,x+1,"➢");
    }
    mvwprintw(area_menu, y, x, "+--------------+");
    mvwprintw(area_menu, y + 1, x, "|  %-12s|", texto);
    mvwprintw(area_menu, y + 2, x, "+--------------+");

    if (selecionada)
        mvwprintw(area_menu, y + 1, x + 1, "➢");
        wattroff(area_menu, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);
}

void renderizar_menu_combate(WINDOW *area_menu, Player* player, OpcoesMenuCombate opcao_hovered)
{
    werase(area_menu);

    mvwprintw(area_menu, 1, 0, "HP:%d/%d", player->vida, player->vida_max);

    // Desenha a barra de vida
    mvwprintw(area_menu,1,10,"[");
    int porcentagem_vida=(player->vida*10/player->vida_max);
    wattron(area_menu, COLOR_PAIR(COR_VIDA));
    for (int x = 11, i = 0; i < porcentagem_vida; i++, x++)
        mvwprintw(area_menu, 1, x, "█");
    wattroff(area_menu, COLOR_PAIR(COR_VIDA));
    mvwprintw(area_menu, 1, 22, "]");

    mvwprintw(area_menu,1, 35, "Lv.%d", player->level);


    desenhar_botao(area_menu, " 1.ATACAR", 3, 0, opcao_hovered == ATACAR);
    desenhar_botao(area_menu, " 2.ITENS", 3, 24, opcao_hovered == ITENS);
    desenhar_botao(area_menu, " 3.MERCY", 6, 0, opcao_hovered == MERCY);
    desenhar_botao(area_menu, " 4.DESISTIR", 6, 24, opcao_hovered == DESISTIR);
    wrefresh(area_menu);
}


void renderizar_combate_ui(CombateUI* ui, Player* player, Inimigo* inimigo)
{
    box(ui->area_boss,0,0);
    box(ui->area_esquiva,0,0);
    desenhar_jogador(ui->area_esquiva, player);
    renderizar_menu_combate(ui->area_menu, player, INVALIDA);
    renderizar_vida_boss(ui->area_vida_boss, inimigo);

    wrefresh(ui->area_vida_boss);
    wrefresh(ui->area_nome_boss);
    wrefresh(ui->area_boss);
    wrefresh(ui->area_esquiva);
    wrefresh(ui->area_menu);
}
void renderizar_vida_boss(WINDOW *area_vida_boss, Inimigo *inimigo)
{
    if (inimigo == NULL)
        return;
    werase(area_vida_boss);
    int barras_totais = 20;
    int barras_cheias = (inimigo->vida * barras_totais) / inimigo->vida_max;

    mvwprintw(area_vida_boss, 0, 0, "HP [");
    wattron(area_vida_boss, COLOR_PAIR(COR_VIDA));
    for (int i = 0; i < barras_cheias; i++)
        mvwprintw(area_vida_boss, 0, 4 + i, "█");
    wattroff(area_vida_boss, COLOR_PAIR(COR_VIDA));
    mvwprintw(area_vida_boss, 0, 4+barras_totais, "] %d/%d", inimigo->vida, inimigo->vida_max);

    wrefresh(area_vida_boss);
}

void desenhar_jogador(WINDOW *area_esquiva, Player *player)
{
    mvwprintw(area_esquiva, player->posicao.y, player->posicao.x, "🤍");
}

void limpar_combate(CombateUI *ui, Inimigo *inimigo);