#ifndef INVENTARIO_H
#define INVENTARIO_H
#include "Atributos.h"

typedef enum TipoConsumivel
{
    POCAO,
    ALIMENTO
} TipoConsumivel;

typedef struct Consumivel
{
    char descricao[50];
    char nome[30];
    int curaVida;
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
    int vida; // VIDA A MAIS QUE A ARMA PODE DAR PARA O PLAYER;
    int dano;
    Atributos atributosarma;
    TipoDano tipo;
} Arma;

typedef struct ParteArmadura
{
    char descricao[150];
    char nome[30];
    int vida; // VIDA A MAIS QUE A ARMADURA PODE DAR PARA O PLAYER;
    Atributos atributosarmadura;

} ParteArmadura;

typedef struct Armadura
{
    ParteArmadura capacete;
    ParteArmadura peitoral;
    ParteArmadura grevas;
    ParteArmadura botas;
} Armadura;

typedef struct Inventario{
    Armadura armadura;
    Arma arma;
    Consumivel consumiveis[4];
} Inventario;






Inventario init_inventario();
Arma criar_arma(char nome[], char descricao[], int dano, int vida, TipoDano tipo, Atributos atributosarma);


#endif
