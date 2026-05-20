#ifndef INIMIGO_H
#define INIMIGO_H


typedef struct Inimigo
{
    int vida;
    AtaqueInimigo ataques[5];
    char nome[40];
} Inimigo;

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

Inimigo *inimigo_criar(int vida, char nome[]);

void inimigo_tomar_dano(Inimigo *inimigo, int dano);

void limpar_inimigo(Inimigo *inimigo);

#endif