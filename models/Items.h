#ifndef ITEMS_H
#define ITEMS_H

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


typedef enum TipoDano{
    MAGICO,
    PERFURACAO,
    CORTE,
    CONTUSAO
} TipoDano ;

typedef struct Arma
{
    char descricao[50];
    char nome[30];
    int dano;
    TipoDano tipo;
} Arma;

typedef struct Armadura
{
    char descricao[50];
    char nome[30];
    int defesa;
} Armadura;

Arma *criar_arma(char nome[], char descricao[], int dano, TipoDano tipo);

#endif