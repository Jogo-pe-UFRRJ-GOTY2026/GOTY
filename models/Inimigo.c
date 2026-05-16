#include "../data_structures/hashtable.h"
#include "../data_structures/arraylist.h"
#include "Inimigo.h"
#include "Ataque.h"
#include <stdlib.h>
#include <string.h>

Inimigo *inimigo_criar(int vida, char nome[])
{
    Inimigo *inimigo = malloc(sizeof(Inimigo));

    strcpy(inimigo->nome, nome);
    inimigo->vida = vida;
    inimigo->ataques = arraylist_init(AtaqueInimigo);

    return inimigo;
}

void inimigo_tomar_dano(Inimigo* inimigo, int dano)
{
    inimigo->vida -= dano;

    if (inimigo->vida < 0)
        inimigo->vida = 0;
}

void limpar_inimigo(Inimigo* inimigo)
{

    free(inimigo);
}