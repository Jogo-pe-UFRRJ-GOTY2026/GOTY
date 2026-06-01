#include "../objects/Inventario.h"
#include "../objects/Player.h"
#include "../utils/utils.h"
#include <stdlib.h>

char* get_sprite_arma(TipoDano tipoDano)
{
    char* opcoes_corte[4]={
        "assets/sprites/loot/axe.txt", 
        "assets/sprites/loot/sabre.txt",
        "assets/sprites/loot/sword1.txt",
        "assets/sprites/loot/sword2.txt",
        }; 
    char* opcoes_perfuracao[4]={
        "assets/sprites/loot/trident.txt", 
        "assets/sprites/loot/spear.txt", 
        "assets/sprites/loot/crossbow.txt", 
        "assets/sprites/loot/bow.txt", 
    }; 
    char* opcoes_contusao[4]={
        "assets/sprites/loot/hammer.txt", 
        "assets/sprites/loot/hammer.txt", 
        "assets/sprites/loot/shield1.txt", 
        "assets/sprites/loot/shield3.txt", 
    } ;
    char* opcoes_magico[4]={
        "assets/sprites/loot/magic_wand.txt", 
        "assets/sprites/loot/magic_staff.txt", 
        "assets/sprites/loot/magic_staff2.txt", 
        "assets/sprites/loot/magic_wand2.txt", 
    };

    switch (tipoDano)
    {
        case MAGICO:
            return opcoes_magico[rand()%4];
        case PERFURACAO:
            return opcoes_perfuracao[rand()%4];
        case CORTE:
            return opcoes_corte[rand()%4];
        case CONTUSAO:
            return opcoes_contusao[rand()%4];

        default:
            break;
    }
}


void gerar_loot(Player* player)
{
    int opcao='1';
    Arma arma1 = criar_arma(player->NumeroAndar, "ajeita as coisas pabluut", "pfvrzin", 5,5, MAGICO);
    Arma arma2 = criar_arma(player->NumeroAndar, "ajeita as coisas pabluut", "pfvrzin", 5,5, MAGICO);
    Arma arma3 = criar_arma(player->NumeroAndar, "ajeita as coisas pabluut", "pfvrzin", 5,5, MAGICO);
    Arma arma_selecionada;
    WINDOW* tela_loot = newwin(getmaxx(stdscr), getmaxy(stdscr), 0, 0);
    
    while(true)
    {
        werase(tela_loot);

        desenhar_sprite(tela_loot, "assets/sprites/loot/chest", 5,0);

        if(opcao=='1')
        {
            wattron(tela_loot, COLOR_PAIR(COR_OPCAO_SELECIONADA));
            arma_selecionada=arma1;
        }
        desenhar_sprite(tela_loot, "assets/sprites/loot/chest", 5, 17);

        if(opcao=='2')
        {
            wattron(tela_loot, COLOR_PAIR(COR_OPCAO_SELECIONADA));
            arma_selecionada=arma1;
        }
        desenhar_sprite(tela_loot, "assets/sprites/loot/chest", 5, 48);

        if(opcao=='3')
        {
            wattron(tela_loot, COLOR_PAIR(COR_OPCAO_SELECIONADA));
            arma_selecionada=arma1;
        }
        desenhar_sprite(tela_loot, "assets/sprites/loot/chest", 5, 79);

        watroff(tela_loot, COLOR_PAIR(COR_OPCAO_SELECIONADA));

        if(opcao==KEY_ENTER || opcao == '\n' || opcao==10)
        {
            player->inventario.arma=arma_selecionada;
            break;
        }


        opcao = wgetch(tela_loot);

    }
    werase(tela_loot);
    delwin(tela_loot);
    tela_loot=NULL;
}