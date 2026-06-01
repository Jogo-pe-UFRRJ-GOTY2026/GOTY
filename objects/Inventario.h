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

typedef struct ArmaBase{
    char nome[64];
    char descricao[256];
    TipoDano tipo;
    int danoBase;
    int vidaBase;
    char sprite[128];
} ArmaBase;


typedef struct Arma
{
    char descricao[50];
    char nome[30];
    int vida; // VIDA A MAIS QUE A ARMA PODE DAR PARA O PLAYER;
    int dano;
    char sprite[128];
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

typedef enum TipoArmadura
{
    CAPACETE,
    PEITORAL,
    GREVAS,
    BOTAS

} TipoArmadura;

Inventario init_inventario();
Arma criar_arma(int capitulo, char nome[], char descricao[], int danoBase, int vidaBase, TipoDano tipo);
Arma gerar_arma_aleatoria(int capitulo);
ParteArmadura criar_armadura(int capitulo, TipoArmadura tipo, char nome[], char descricao[], int vidaBase);


#endif
