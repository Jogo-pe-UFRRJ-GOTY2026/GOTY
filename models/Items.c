#include "Items.h"
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"

Arma* criar_arma(char nome[], char descricao[],int dano, TipoDano tipo)
{
    Arma* arma = malloc(sizeof(Arma));
    if (arma == NULL)
        exit_with_error(Exception_AllocationError);

    strcpy(arma->nome, nome);
    strcpy(arma->descricao, descricao);
    arma->dano = dano;
    arma->tipo = tipo;
    return arma;
}