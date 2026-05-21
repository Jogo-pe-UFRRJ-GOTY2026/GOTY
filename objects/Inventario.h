#ifndef INVENTARIO_H
#define INVENTARIO_H
#include "../data_structures/hashtable.h"

typedef enum TipoConsumivel
{
    POCAO,
    ALIMENTO
} TipoConsumivel;

typedef struct Consumivel
{
    char descricao[50];
    char nome[30];
    int vida;
    int bonus;
    TipoConsumivel tipo;
} Consumivel;

typedef enum TipoDano
{
    MAGICO,
    PERFURACAO,
    CORTE,
    CONTUSAO
} TipoDano;

typedef struct Arma
{
    char descricao[50];
    char nome[30];
    int dano;
    TipoDano tipo;
} Arma;

typedef struct ParteArmadura
{
    char descricao[150];
    char nome[30];
    int defesa;
} ParteArmadura;

typedef struct Armadura
{
    ParteArmadura capacete;
    ParteArmadura peitoral;
    ParteArmadura grevas;
    ParteArmadura botas;
} Armadura;



typedef struct Inventario{
    Armadura* armadura;
    Arma* arma;
    hash_table* consumiveis;
} Inventario;






Inventario* init_inventario();
Arma *criar_arma(char nome[], char descricao[], int dano, TipoDano tipo);
int get_defesa(Inventario *inv, const char *slot);


#endif
