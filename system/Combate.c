#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "../objects/Inimigo.h"
#include "../objects/Player.h"

void iniciar_combate(Player* player, char tecla_pressionada)
{
    int max_y, max_x;

    WINDOW* boss_area;//=criar_area_boss();
    WINDOW* campo;//=criar_arena_de_batalha();

    // A ideia é desenhar o boss na janela campo

    //mover_player(player, get_direcao(tecla_pressionada));
}

bool ataque_colidiu(Player *player, AtaqueInimigo* Ataque);

void spawn_atack(AtaqueInimigo *Ataque);

bool rodada()
{
    // fala do boss
    // 1 sequencia de um ataque especifico
    // ação do player
    // fala do boss

}





WINDOW *criar_area_boss(int altura, int comprimento, int y_comeco, int x_comeco)
{
    return newwin(altura, comprimento, y_comeco, x_comeco);
}

WINDOW* criar_arena_de_batalha(int altura,int comprimento,int y_comeco, int x_comeco)
{
    return newwin(altura, comprimento, y_comeco, x_comeco);
}

WINDOW *criar_menu_batalha( int y_comeco, int x_comeco)
{
    int altura=10;
    int comprimento = 60;
    return newwin(altura, comprimento, y_comeco, x_comeco);
}

void limpar_combate(WINDOW* area_boss, WINDOW* arena, WINDOW* menu, Inimigo* inimigo);