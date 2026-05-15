#include <stdlib.h>
#include <time.h>
#include "utils.h"


const char* get_error_message(Exceptions ex)
{
    switch(ex)
    {
    case AllocationError: return "Falha ao alocar recurso com malloc ou calloc";
    case KeyError: return "Falha ao acessar chave em um hashmap";
    case NotImplementedError: return "Falha, função não implementada";
    case IndexError: return "Falha ao acessar indice de array";
    case OverflowError: return "Falha ao realizar operação aritmitica, acontecimento de Overflow";
    default: "Aconteceu um erro:";break;
    }
}

int generate_damage_roll()
{
    srand(time(NULL));

    return rand()%20 + 5;
}

void exit_with_error(Exceptions ex)
{
    perror(get_message(ex));
    exit(1);
}