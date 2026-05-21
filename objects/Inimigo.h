#ifndef INIMIGO_H
#define INIMIGO_H

typedef enum TipoAtaque
{
    LINHA,
    AREA,
    PONTO,
    ESFERA,
    DISPERSAO,
    BULLET,
} TipoAtaque;

typedef struct AtaqueInimigo
{
    int x;
    int y;
    int dano;
    int hit_box;
    TipoAtaque tipo_ataque;
} AtaqueInimigo;

typedef struct Inimigo
{
    int vida;
    int mercy; //quando chegar a 100, pode poupar a batalha.
    AtaqueInimigo ataques[5];
    char nome[40];
} Inimigo;






Inimigo *inimigo_criar(int vida, char nome[], int mercy);

void inimigo_tomar_dano(Inimigo *inimigo, int dano);

void inimigo_tomar_mercy(Inimigo *inimigo, int dano);

void limpar_inimigo(Inimigo *inimigo);

#endif