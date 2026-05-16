#ifndef INIMIGO_H
#define INIMIGO_H

#include "../data_structures/hashtable.h"
#include "../data_structures/arraylist.h"

typedef struct Inimigo
{
    int vida;
    ArrayList* ataques;
    char nome[40];
} Inimigo;

Inimigo *inimigo_criar(int vida, char nome[]);

void inimigo_tomar_dano(Inimigo *inimigo, int dano);

void limpar_inimigo(Inimigo *inimigo);

#endif