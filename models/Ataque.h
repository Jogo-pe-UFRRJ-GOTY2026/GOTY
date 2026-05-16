#ifndef ATAQUE_H
#define ATAQUE_H

typedef enum TipoAtaque
{
    LINHA,
    AREA,
    PONTO,
    ESFERA,
    DISPERSAO,
} TipoAtaque;

typedef struct AtaqueInimigo{
    int x;
    int y;
    int dano;
    int hit_box;
    TipoAtaque tipo_ataque;
} AtaqueInimigo;

#endif