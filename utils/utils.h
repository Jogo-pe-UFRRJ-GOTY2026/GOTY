#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

typedef enum Exceptions
{
    Exception_AllocationError = 1,
    Exception_KeyError,
    Exception_NotImplementedError,
    Exception_IndexError,
    Exception_OverflowError
} Exceptions;

int gerar_rolagem_dano();
int gerar_chance_de_evasao_do_inimigo();
void exit_with_error(Exceptions ex);

typedef enum Direcao {
    ESQUERDA,
    DIREITA,
    BAIXO,
    CIMA,
    DIRECAO_INVALIDA
} Direcao;

#define len(arr) (sizeof(arr)/sizeof(arr[0]))

#endif