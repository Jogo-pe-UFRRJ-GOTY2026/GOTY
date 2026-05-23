#ifndef INIMIGO_H
#define INIMIGO_H
#include <stdbool.h>

typedef enum TipoAtaque
{
    LINHA,
    AREA,//Retangulo
    ESFERA,
    BULLET,
    PAREDE,
    LASER,
} TipoAtaque;
typedef enum DirecaoAtaque
{
    HORIZONTAL,
    VERTICAL,
    DIAGONAL,
    PONTO
} DirecaoAtaque;

typedef struct AtaqueInimigo
{
    int x;
    int y;
    int dano;
    int vel_horizontal;
    int vel_vertical;
    char ataque_sprite[32];
    int hit_box;
    DirecaoAtaque direcao;
    TipoAtaque tipo_ataque;
    bool ativo;
} AtaqueInimigo;

typedef struct sprite_size{
    int x;
    int y;
} Sprite_size;

typedef struct Inimigo
{
    int vida;
    int mercy; //quando chegar a 100, pode poupar a batalha.
    char sprite[50];
    Sprite_size sprite_size;
    const char *dialogos[6];
    AtaqueInimigo ataques[10];
    char nome[40];
} Inimigo;

Inimigo *criar_inimigo(int vida, char nome[], const char *sprite_location, Sprite_size size, int mercy);
AtaqueInimigo criar_ataque(TipoAtaque tipo, int dano, const char *ataque_sprite, int hitbox, DirecaoAtaque direcao);
void inimigo_tomar_dano(Inimigo *inimigo, int dano);
void inimigo_tomar_mercy(Inimigo *inimigo, int dano);
void limpar_inimigo(Inimigo *inimigo);

#endif