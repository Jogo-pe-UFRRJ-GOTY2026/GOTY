#include "Inventario.h"
#include "../data_structures/hashtable.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>


Inventario* init_inventario()
{
    Inventario* inventario = malloc(sizeof(Inventario));
    if (inventario == NULL)
        exit_with_error(Exception_AllocationError);

    inventario->armadura = hash_table_init();
    if (inventario->armadura == NULL)
        exit_with_error(Exception_AllocationError);


    ParteArmadura peitoral, luva, bota;
    peitoral.defesa = 2;
    strcpy(peitoral.descricao, "peitoral inicial de baixa qualidade, protege contra ataques em pontos vitais como coração e pulmão");
    strcpy(peitoral.nome, "peitoral inicial");

    luva.defesa=1;
    strcpy(luva.descricao,"manoplas iniciais de baixa qualidade, conferem proteção e calor as mãos");
    strcpy(luva.nome,"manoplas iniciais");

    bota.defesa=1;
    strcpy(bota.descricao,"botas iniciais de baixa qualidade, conferem proteção e protegem contra queimaduras nos pés");
    strcpy(bota.nome, "botas iniciais");




    inventario->arma = criar_arma("Espada inicial", "Espada inicial fraca",4, CORTE);

    inventario->consumiveis = hash_table_init();
    if (inventario->consumiveis == NULL)
        exit_with_error(Exception_AllocationError);

    return inventario;
}











Arma *criar_arma(char nome[], char descricao[], int dano, TipoDano tipo)
{
    Arma *arma = malloc(sizeof(Arma));
    if (arma == NULL)
        exit_with_error(Exception_AllocationError);

    strcpy(arma->nome, nome);
    strcpy(arma->descricao, descricao);
    arma->dano = dano;
    arma->tipo = tipo;

    return arma;
}

int get_defesa(Inventario *inv, const char *slot)
{
    return ((Armadura *)hash_table_get(inv->armadura, slot))->defesa;
}