#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "Combate.h"
#include "../objects/Inimigo.h"
#include "../objects/Player.h"
#include "../utils/utils.h"


void iniciar_combate(Player* player, Inimigo* inimigo)
{
    CombateUI ui;
    int max_x=getmaxy(stdscr);
    int esquiva_max_y, esquiva_max_x;

    //BOSS
    mvprintw(2,(max_x/2)-10,"%s", inimigo->nome);
    ui.area_boss = newwin(inimigo->sprite_size.y + 2, inimigo->sprite_size.x + 2, 5, 0);
    desenhar_sprite(ui.area_boss, inimigo->sprite, 1, 1);

    //DODGE AREA E PLAYER
    ui.area_esquiva = newwin(16,56, 6, inimigo->sprite_size.x+20);

    getmaxyx(ui.area_esquiva, esquiva_max_y, esquiva_max_x);
    player->posicao.x=esquiva_max_x/2;
    player->posicao.y=esquiva_max_y/2;



    ui.area_menu = newwin(20,56, 22,inimigo->sprite_size.x+20);

    renderizar_combate_ui(&ui, player);

    wgetch(ui.area_esquiva);

    EstagioCombate estado_atual = FASE_DIALOGO1;


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

    mvwprintw(area_menu, 1, 3, "HP:%d/%d", player->vida, player->vida_max);

    // Desenha a barra de vida
    mvwprintw(area_menu,1,15,"[");
    int porcentagem_vida=(player->vida_max/player->vida*10);
    for (int x = 16, i = 0; i < porcentagem_vida; i++, x++)
        mvwprintw(area_menu, 1, x, "█");
    mvwprintw(area_menu,1,26,"]");

    mvwprintw(area_menu,1, 35, "Lv.%d", player->level);


    desenhar_botao(area_menu, "1.ATACAR", 3, 2, opcao_hovered == ATACAR);
    desenhar_botao(area_menu, "2.ITENS", 3, 35, opcao_hovered == ITENS);
    desenhar_botao(area_menu, "3.MERCY", 6, 2, opcao_hovered == MERCY);
    desenhar_botao(area_menu, "4.DESISTIR", 6, 35, opcao_hovered == DESISTIR);
}











void renderizar_combate_ui(CombateUI* ui, Player* player)
{
    box(ui->area_boss,0,0);

    box(ui->area_esquiva,0,0);
    desenhar_jogador(ui->area_esquiva, player);
    renderizar_menu_combate(ui->area_menu, player, INVALIDA);

    wrefresh(ui->area_boss);
    wrefresh(ui->area_esquiva);
    wrefresh(ui->area_menu);
}






void desenhar_jogador(WINDOW *area_esquiva, Player *player)
{
    mvwprintw(area_esquiva, player->posicao.y, player->posicao.x, "🤍");
}

bool ataque_colidiu(Player *player, AtaqueInimigo *Ataque);

void spawnar_ataque(AtaqueInimigo *Ataque);

bool rodada()
{
    // fala do boss
    // 1 sequencia de um ataque especifico
    // ação do player
    // fala do boss

}

void limpar_combate(CombateUI *ui, Inimigo *inimigo);