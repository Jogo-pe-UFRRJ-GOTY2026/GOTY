#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include "utils.h"
#include "../objects/Player.h"
#include <string.h>

char* get_error_message(Exceptions ex)
{
    switch(ex)
    {
        case Exception_AllocationError: return "Falha ao alocar recurso com malloc ou calloc";
        case Exception_KeyError: return "Falha ao acessar chave em um hashmap";
        case Exception_NotImplementedError: return "Falha, função não implementada";
        case Exception_FileReadWriteError: return "Falha, erro ao ler arquivo do sprite";
        case Exception_IndexError: return "Falha ao acessar indice de array";
        case Exception_OverflowError: return "Falha ao realizar operação aritmitica, acontecimento de Overflow";
        default: return "Aconteceu um erro:";break;
    }
}

int gerar_rolagem_dano()
{

    return (rand()%20 + 1);
}

int gerar_rolagem_mercy()
{

    return (rand()%15 + 1);

}
int escolher_ataque()
{
    return rand() % 5;
}
int gerar_chance_de_evasao_do_inimigo()
{

    return (rand() % 100 + 1);
}

void exit_with_error(Exceptions ex)
{
    perror(get_error_message(ex));
    exit(1);
}

void iniciar_cores()
{
    const int STANDARD_BACKGROUND = -1;
    short COLOR_CRIMSON=16;
    init_color(COLOR_CRIMSON, 710, 192, 251);

    init_pair(COR_TEXTO_MORTE, COLOR_CRIMSON, STANDARD_BACKGROUND);
    init_pair(COR_OPCAO_SELECIONADA, COLOR_YELLOW, STANDARD_BACKGROUND);
    init_pair(COR_VIDA, COLOR_RED, STANDARD_BACKGROUND);
    init_pair(COR_NOME_BOSS, COLOR_WHITE, STANDARD_BACKGROUND);
}

void desenhar_sprite(WINDOW *win, const char *nome_arquivo, int y_inicial, int x_inicial)
{
    FILE *f = fopen(nome_arquivo, "r");
    if(f==NULL)
        perror(get_error_message(Exception_FileReadWriteError));

    char linha[200];
    int i =0;

    while(fgets(linha, sizeof(linha), f))
    {
        linha[strcspn(linha, "\n")] = '\0';

        mvwprintw(win,y_inicial+i,x_inicial,"%s", linha);
        i+=1;
    }
    fclose(f);
}

void slow_mvwprintw(WINDOW* win, char* fala, const int y, int x, int delay_ms)
{
    for(int i=0;fala[i]!='\0';i++)
    {
        mvwaddch(win, y, x + i, fala[i]);
        wrefresh(win);
        napms(delay_ms);
    }
}

void mostrar_tela_morte(Player* player)
{
    WINDOW* tela_morte = newwin(getmaxy(stdscr), getmaxx(stdscr),0,0);

    wattron(tela_morte, COLOR_PAIR(COR_TEXTO_MORTE) | A_BOLD);
    mvwprintw(tela_morte, 16, 70, "██    ██  ██████  ██    ██     ██████  ██ ███████ ██████  ");
    mvwprintw(tela_morte, 17, 70, " ██  ██  ██    ██ ██    ██     ██   ██ ██ ██      ██   ██");
    mvwprintw(tela_morte, 18, 70, "  ████   ██    ██ ██    ██     ██   ██ ██ █████   ██   ██");
    mvwprintw(tela_morte, 19, 70, "   ██    ██    ██ ██    ██     ██   ██ ██ ██      ██   ██ ");
    mvwprintw(tela_morte, 20, 70, "   ██     ██████   ██████      ██████  ██ ███████ ██████ ");

    wattroff(tela_morte, COLOR_PAIR(COR_TEXTO_MORTE) | A_BOLD);
    wrefresh(tela_morte);
    napms(2000);

    werase(tela_morte);

    int medida_lembrancas = player->medidor_lembranca[Reaper], indice_dialogo;
    player->medidor_lembranca[Reaper]+=1;

    char *dialogos[] = {
        "Mais um falhando... Terei de te levar de volta ao inicio.",
        "Vejo que falhou de novo. Pretende continuar tentando?",
        "De novo? Você realmente não desiste.",
        "Outra queda... A Torre deve gostar de você.",
        "Você continua voltando. Interessante.",
        "Quantas vezes pretende cair antes de chegar ao topo?",
        "Você já parece diferente de quando chegou aqui.",
        "A maioria teria desistido.",
        "A Torre tira algo de você a cada retorno.",
        "Ainda consegue lembrar por que está subindo?",
        "Olá de novo, %s... Voltando para baixo?"
    };

    if (medida_lembrancas == 0)
        indice_dialogo = 0;
    else if (medida_lembrancas == 1)
        indice_dialogo = 1;
    else if (medida_lembrancas == 2)
        indice_dialogo = 2;
    else if (medida_lembrancas == 3)
        indice_dialogo = 3;
    else if (medida_lembrancas == 4)
        indice_dialogo = 4;
    else if (medida_lembrancas == 5)
        indice_dialogo = 5;
    else if (medida_lembrancas == 6)
        indice_dialogo = 6;
    else if (medida_lembrancas == 7)
        indice_dialogo = 7;
    else if (medida_lembrancas == 8)
        indice_dialogo = 8;
    else if (medida_lembrancas == 9)
        indice_dialogo = 9;
    else
        indice_dialogo = 10;
    desenhar_sprite(tela_morte, "assets/sprites/bosses/reaper.txt",5,15);
    wrefresh(tela_morte);
    napms(1000);
    slow_mvwprintw(tela_morte, dialogos[indice_dialogo], 10, 70, 30);
    wrefresh(tela_morte);
    napms(1000);
    
    delwin(tela_morte);
}