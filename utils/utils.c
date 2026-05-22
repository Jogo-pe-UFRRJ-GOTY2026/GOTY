#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include "utils.h"


char* get_error_message(Exceptions ex)
{
    switch(ex)
    {
        case Exception_AllocationError: return "Falha ao alocar recurso com malloc ou calloc";
        case Exception_KeyError: return "Falha ao acessar chave em um hashmap";
        case Exception_NotImplementedError: return "Falha, função não implementada";
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
    // Para criar cores mais costomizadas, ver  init_color, e can_change_color() que retorna true se o terminal suporta essa costumizacao
    const int STANDARD_BACKGROUND = -1;
    // init_pair(id, cor_foreground ,cor_background)
    init_pair(COR_OPCAO_SELECIONADA, COLOR_YELLOW, STANDARD_BACKGROUND);
    init_pair(COR_NOME, COLOR_CYAN, STANDARD_BACKGROUND);
    init_pair(COR_NOME, COLOR_RED, STANDARD_BACKGROUND);
}