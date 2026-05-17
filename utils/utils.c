#include <stdlib.h>
#include <time.h>
#include "utils.h"


const char* get_error_message(Exceptions ex)
{
    switch(ex)
    {
        case Exception_AllocationError: return "Falha ao alocar recurso com malloc ou calloc";
        case Exception_KeyError: return "Falha ao acessar chave em um hashmap";
        case Exception_NotImplementedError: return "Falha, função não implementada";
        case Exception_IndexError: return "Falha ao acessar indice de array";
        case Exception_OverflowError: return "Falha ao realizar operação aritmitica, acontecimento de Overflow";
        default: "Aconteceu um erro:";break;
    }
}

int gerar_rolagem_dano()
{
    srand(time(NULL));

    return (rand()%20 + 1);
}

int gerar_chance_de_evasao_do_inimigo()
{
    srand(time(NULL));

    return (rand() % 100 + 1);
}

void exit_with_error(Exceptions ex)
{
    perror(get_message(ex));
    exit(1);
}

