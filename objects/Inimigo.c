#include "Inimigo.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>

AtaqueInimigo *criar_ataque(TipoAtaque tipo, int dano, int hitbox)
{
    AtaqueInimigo* ataque = malloc(sizeof(AtaqueInimigo));
    if(ataque==NULL)
        exit_with_error(Exception_AllocationError);

    ataque->tipo_ataque=tipo;
    ataque->dano=dano;
    ataque->hit_box=hitbox;

    return ataque;
}

Inimigo *inimigo_criar(int vida, char nome[], char sprite_location[], AtaqueInimigo *ataques[5], int mercy)
{
    Inimigo *inimigo = malloc(sizeof(Inimigo));

    strcpy(inimigo->nome, nome);
    inimigo->vida = vida;
    inimigo->mercy = mercy;
    strcpy(inimigo->sprite, sprite_location);
    for (int i = 0; i < 5; i++)
    {
        inimigo->ataques[i] = (*ataques[i]);
        free(ataques[i]);
    }
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