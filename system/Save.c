#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../objects/Player.h"
#include "../objects/Inventario.h"
#include "../utils/utils.h"
#include "../chapters/CAPITULO.h"

#define NUM_OF_ELEMENTS 1
Player* carregar_salvamento()
{

    FILE *save_file = fopen("saves/player_save.bin", "rb");
    if(save_file==NULL)
    {
        //perror("Test Error: Erro ao ler arquivo de salvamento");
        return NULL;
    }

    Player* player = malloc(sizeof(Player));
    if(player==NULL)
    {
        //perror("Test Error: Erro ao alocar estrutura player");
        fclose(save_file);
        return NULL;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if(fread(player, sizeof(Player), NUM_OF_ELEMENTS, save_file)!=NUM_OF_ELEMENTS)
    {
        //perror("Test Error: Erro ao ler estrutura player do arquivo de salvamento");
        free(player);
        fclose(save_file);
        return NULL;
    }
    fclose(save_file);
    

    return player;
}

bool salvar_jogo(Player *player)
{
    FILE* save_file = fopen("saves/player_save.bin", "wb");
    if(save_file==NULL)
    {
        perror("Test Error: Erro ao criar arquivo de salvamento");
        return false;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if(fwrite(player, sizeof(Player), NUM_OF_ELEMENTS, save_file) != NUM_OF_ELEMENTS)
    {
        perror("Test Error: Erro ao escrever estrutura player no arquivo de salvamento");
        fclose(save_file);
        return false;
    }
    fclose(save_file);
    return true;


}

void ponto_save(Player* player)
{
    WINDOW* ponto_save_window = newwin(getmaxy(stdscr),getmaxx(stdscr), 0 ,0);
    keypad(ponto_save_window, TRUE); 
    bool descansando = true;
    int tecla;

    while(descansando)
    {
        werase(ponto_save_window);
        box(ponto_save_window, 0 , 0);
        desenhar_sprite(ponto_save_window,"assets/sprites/buildings/save_point.txt",5,5);

        tecla = wgetch(ponto_save_window);

        switch(tecla)
        {
            case '1':
                salvar_jogo(player);
                wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE)| A_BOLD);
                mvwprintw(ponto_save_window, getmaxy(ponto_save_window)-5, 5, "Progresso salvo com sucesso");
                wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
                wrefresh(ponto_save_window);
                napms(1000);

                break;

            case '2':
                salvar_jogo(player);
                wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE)| A_BOLD);
                mvwprintw(ponto_save_window, getmaxy(ponto_save_window)-3, 5, "Progresso salvo com sucesso. Fechando jogo...");
                wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
                wrefresh(ponto_save_window);
                napms(1500);
                apagar_janela(ponto_save_window);
                endwin();
                free(player);
                exit(EXIT_SUCCESS);

            case '3':
                werase(ponto_save_window);
                desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                char *opcoes_dialogo[3];
                int num_perguntas; //max 3, min 0
                int ja_realizou_pergunta[3] = {0};
                int opcao;

                switch (player->NumeroAndar)
                {
                    // adicionar aqui os dialogos possiveis com o vigia em cada andar
                    case Andar1:
                        break;

                    case Andar2:
                        break;

                    case Andar3:
                        break;

                    case Andar4:
                        break;

                    case Andar5:
                        break;
                }
                bool conversando = true;
                while (conversando)
                {
                    werase(ponto_save_window);
                    box(ponto_save_window, 0, 0);
                    desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                    mvwprintw(ponto_save_window, 28, 6, "[Vigia]");

                    for (int i = 0; i < num_perguntas; i++)
                    {
                        if (ja_realizou_pergunta[i])
                            wattron(ponto_save_window, COLOR_PAIR(COR_OPCAO_INVALIDA));
                        mvwprintw(ponto_save_window, 30 + i, 6, opcoes_dialogo[i]);
                        wattrset(ponto_save_window, A_NORMAL);
                    }
                    wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE));
                    mvwprintw(ponto_save_window, getmaxy(ponto_save_window) - 3, 4, "Pressione ENTER para se despedir");
                    wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE));
                    wrefresh(ponto_save_window);
                    opcao = wgetch(ponto_save_window);

                    if (opcao == KEY_ENTER || opcao == '\n' || opcao == 10)
                    {
                        conversando = false;
                        break;
                    }
                    if (!ja_realizou_pergunta[opcao])
                    {
                        ja_realizou_pergunta[opcao] = 1;
                        werase(ponto_save_window);
                        box(ponto_save_window, 0, 0);
                        desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                        mvwprintw(ponto_save_window, 28, 6, "[Vigia]");
                        wrefresh(ponto_save_window);



                        if(opcao == KEY_ENTER || opcao == '\n' || opcao == 10)
                        {
                            conversando = false;
                            break;
                        }
                    }
                }
                break;

            case '4':
                slow_mvwprintw(ponto_save_window, "Você levanta e volta a Torre", getmaxy(ponto_save_window)-3, 5, 30);
                descansando = false;
                break;
        }
 
        

            
  
 
    }


}