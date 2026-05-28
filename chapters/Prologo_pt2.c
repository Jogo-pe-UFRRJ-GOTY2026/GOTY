#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "CAPITULO.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"
#include "../system/Combate.h"
#include "../utils/utils.h"


void Prologo_pt2(Player* player)
{
    WINDOW* tela_prologopt2 = newwin(getmaxy(stdscr), getmaxx(stdscr),0,0);
    //Falas antes do combate
    








    
    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 10, "█▙▔▔▔", 1, HORIZONTAL, DIREITA_ESQUERDA, 2),
        criar_ataque(BULLET, 2, "†", 1, VERTICAL, CIMA_BAIXO, 3),
        criar_ataque(BULLET, 3, "*", 1, HORIZONTAL, ESQUERDA_DIREITA, 5)};

    Sprite_size size;
    size.x=33;
    size.y=20;

    Inimigo *hollow_knight = criar_inimigo(100, "Hollow Knight", "assets/sprites/bosses/hollow.txt",size, 40, 20);
    
    //Falas do combate
    //Mercy
    hollow_knight->dialogos[0]="Você... está tendo PIEDADE de mim?!?!";
    hollow_knight->dialogos[1]="Vai continuar sendo ‘superior’ contra mim?”";
    //Ataque
    hollow_knight->dialogos[2]="Não fez nem cosquinha";
    hollow_knight->dialogos[3]="Acha mesmo que pode me vencer?";
    hollow_knight->dialogos[4]="Eu não serei derrotado por você";
    hollow_knight->dialogos[5]="Impressionante…”";


    hollow_knight->numero_ataques=len(ataques);
    for(int i=0;i<hollow_knight->numero_ataques;i++)
    {
        hollow_knight->ataques[i] = ataques[i];
    }

    EstadoRodada resultado_combate = iniciar_combate(player, hollow_knight);
    if(resultado_combate==VITORIA && hollow_knight->vida<=0)
    {
        mvwprintw(tela_prologopt2,1,1,"...boa sorte, viajante.");
        mvwprintw(tela_prologopt2,2,1,"...talvez você chegue mais longe do que eu.");

    }
    else if(resultado_combate==VITORIA && hollow_knight->vida>=0)
    {

    }
    else //(resultado_combate==DERROTA)
    {

    }
    
    //matou ele




}