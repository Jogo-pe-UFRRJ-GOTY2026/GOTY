//Dialogo, e tutorial de combate
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "CAPITULO.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"

void  Introducao();

Player* Prologo()
{

    Introducao();



    return Tela_criacao();

}






// ================================================================================= //
//                              Criação de personagem                                // 
// ================================================================================= //
Player* Tela_criacao()
{
    int altura_tela, largura_tela;
    getmaxyx(stdscr, altura_tela, largura_tela);

    WINDOW *criacao_personagem_window = newwin(0, 0, 0, 0);

    char nome[50]="";
    int tamanho_atual_nome=0;
    char *opcoes[] = {"Masculino","Feminino"};
    int largura_box = largura_tela/3;
    int start_x = 5;
    box(criacao_personagem_window,0,0);
    int genero_selecionado = Masculino;
    int tecla;
    keypad(criacao_personagem_window, TRUE);
    echo(); // Permitiria os caracteres a aparecerem na tela, de

    while (true)
    {
        werase(criacao_personagem_window);

        // ====================================================== TITULO =====================================================


        mvwprintw(criacao_personagem_window, 3, largura_tela / 3, "██████╗██████╗ ██╗ █████╗  ██████╗ █████╗  ██████╗     ██████╗ ███████╗");   
        mvwprintw(criacao_personagem_window, 4, largura_tela / 3, "██╔════╝██╔══██╗██║██╔══██╗██╔════╝██╔══██╗██╔═══██╗    ██╔══██╗██╔════╝");  
        mvwprintw(criacao_personagem_window, 5, largura_tela / 3, "██║     ██████╔╝██║███████║██║     ███████║██║   ██║    ██║  ██║█████╗"); 
        mvwprintw(criacao_personagem_window, 6, largura_tela / 3, "██║     ██╔══██╗██║██╔══██║██║     ██╔══██║██║   ██║    ██║  ██║██╔══╝ ");    
        mvwprintw(criacao_personagem_window, 7, largura_tela / 3, "╚██████╗██║  ██║██║██║  ██║╚██████╗██║  ██║╚██████╔╝    ██████╔╝███████╗");  
        mvwprintw(criacao_personagem_window, 8, largura_tela / 3, " ╚═════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝     ╚═════╝ ╚══════╝");    

        mvwprintw(criacao_personagem_window, 10, largura_box, "██████╗ ███████╗██████╗ ███████╗ ██████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗███╗   ███╗");
        mvwprintw(criacao_personagem_window, 11, largura_box, "██╔══██╗██╔════╝██╔══██╗██╔════╝██╔═══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝████╗ ████║");
        mvwprintw(criacao_personagem_window, 12, largura_box, "██████╔╝█████╗  ██████╔╝███████╗██║   ██║██╔██╗ ██║███████║██║  ███╗█████╗  ██╔████╔██║");
        mvwprintw(criacao_personagem_window, 13, largura_box, "██╔═══╝ ██╔══╝  ██╔══██╗╚════██║██║   ██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██║╚██╔╝██║");
        mvwprintw(criacao_personagem_window, 14, largura_box, "██║     ███████╗██║  ██║███████║╚██████╔╝██║ ╚████║██║  ██║╚██████╔╝███████╗██║ ╚═╝ ██║");
        mvwprintw(criacao_personagem_window, 15, largura_box, "╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝");

        mvwprintw(criacao_personagem_window, 20, start_x, "+------------------------------------------------------------+");
        mvwprintw(criacao_personagem_window, 21, start_x, "|   ... Você acorda                                          |");
        mvwprintw(criacao_personagem_window, 22, start_x, "|   Digite seu nome: %s", nome);
        mvwprintw(criacao_personagem_window, 22, start_x+61, "|");

        mvwprintw(criacao_personagem_window, 23, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 24, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 25, start_x, "|   Genero:                                                  |");


        // =====================================================
        // OPCOES GENERO
        // =====================================================
        for(int i=0,y=26;i<2;i++,y++)
        {
            if(i==genero_selecionado)
            {
                mvwprintw(criacao_personagem_window, y, start_x,"|");
                wattron(criacao_personagem_window, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Ativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(criacao_personagem_window, y, start_x + 1, "     ➢  ⦗%s⦘", opcoes[i]);
                wattroff(criacao_personagem_window, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Desativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(criacao_personagem_window, y, start_x + 61, "|");
            }
            else
            {
                mvwprintw(criacao_personagem_window, y, start_x, "|         %s ", opcoes[i]);
                mvwprintw(criacao_personagem_window, y, start_x + 61, "|");
            }          
        }
        mvwprintw(criacao_personagem_window, 28, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 29, start_x, "| Enter para confirmar                                       |");
        mvwprintw(criacao_personagem_window, 30, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 31, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 32, start_x, "+------------------------------------------------------------+");




        // Desenhando o sprite

        switch(genero_selecionado)
        {
            case Masculino:
                desenhar_sprite(criacao_personagem_window, "assets/sprites/player/sprite_masculino_terminalfix.txt", 17, 85);
                break;
            case Feminino:
                desenhar_sprite(criacao_personagem_window, "assets/sprites/player/sprite_feminino_terminalfix.txt", 17, 85);
                break;
            default:
                break; 
        }


        wrefresh(criacao_personagem_window);

        // TRATANDO DO INPUT
        tecla = wgetch(criacao_personagem_window); // pega o caracter
        if(tecla==10 || tecla == KEY_ENTER)
        {
            break;
        }
        else if (tecla == KEY_DOWN)
        {
            if (genero_selecionado == Feminino)
                genero_selecionado = Masculino;
            else
                genero_selecionado += 1;
        }
        else if (tecla == KEY_UP)
        {
            if (genero_selecionado == Masculino)
                genero_selecionado = Feminino;
            else
                genero_selecionado -= 1;
        }
        else if(tecla==KEY_BACKSPACE || tecla==127 || tecla==8)
        {
        if(tamanho_atual_nome>0)
           {
                tamanho_atual_nome-=1;
                nome[tamanho_atual_nome]='\0';
           } 
        }
        else if(' '<=tecla && tecla<='~')
        {
            if(tamanho_atual_nome < sizeof(nome)-1)
            {
                nome[tamanho_atual_nome]=tecla;
                tamanho_atual_nome+=1;
                nome[tamanho_atual_nome] = '\0';
            }
        }


    }
    delwin(criacao_personagem_window);
    Player* player = criar_player(nome, genero_selecionado);
    return player;
}



// ================================================================================= //
//                              Introdução da história                               // 
// ================================================================================= //
void  Introducao()
{
    int altura, largura;
    getmaxyx(stdscr, altura, largura);

    WINDOW* tela_apresentacao=newwin(0,0,0,0);
    char* texto[]={"Dizem que existe uma saída...", "Dizem que ela fica no topo...", "Mas ninguém volta para confirmar."};
    for(int i=0;i<3;i++) //troca o 3 pelo numero de strings no texto
    {
        mvwprintw(tela_apresentacao,2, largura/3,"%s" ,texto[i] );

        for(int x=5;x<118;x++) mvwprintw(tela_apresentacao, 5,x,"-");
        for(int x=5;x<118;x++) mvwprintw(tela_apresentacao, 40,x,"-");

        for(int y=5;y<111;y++)
        {
            if(y==5) mvwprintw(tela_apresentacao, y,3,"+");
            else mvwprintw(tela_apresentacao, y,3,"|");
        } 

        for(int y=5;y<111;y++)
        {
            if(y==5) mvwprintw(tela_apresentacao, y,118,"+");
            else mvwprintw(tela_apresentacao, y,118,"|");
        } 

        desenhar_sprite(tela_apresentacao, "assets/sprites/buildings/tower.txt", 6, 4);



        wrefresh(tela_apresentacao);
        napms(2000); //espera 2 segundos
    }
    delwin(tela_apresentacao);

}