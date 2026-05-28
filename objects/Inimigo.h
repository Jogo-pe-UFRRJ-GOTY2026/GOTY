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
    PONTO
} DirecaoAtaque;

typedef enum SentidoAtaque
{
    DIREITA_ESQUERDA,
    ESQUERDA_DIREITA,
    CIMA_BAIXO,
    BAIXO_CIMA
} SentidoAtaque;

typedef enum lembrancas_
{
    Hollow_Knight,
    Centaur,
    Demon,
    Cerberus,
    Dragon,
    Fallen_King,
    Vigia,
    Reaper
} Lembrancas;

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
    SentidoAtaque sentido;
    bool ativo;
    //PMI
    int tick_vida;
    int tick_movimento;
    int velocidade; // opcional mas recomendado, quanto menor mais rapido
} AtaqueInimigo;

typedef struct sprite_size{
    int x;
    int y;
} Sprite_size;

typedef struct Inimigo
{
    int vida;
    int vida_max;
    int mercy; //quando chegar a 100, pode poupar a batalha.
    char sprite[50];
    Sprite_size sprite_size;
    int numero_ataques;
    int tempo_por_rodada;
    char *dialogos_ataque[5];
    char *dialogos_mercy[2];
    Lembrancas id;
    char *dialogo_reever_player;
    AtaqueInimigo ataques[10];
    char nome[40];
} Inimigo;

Inimigo *criar_inimigo(int vida, char nome[], const char *sprite_location, Sprite_size size, int mercy, int tempo_por_rodada, Lembrancas Id);
AtaqueInimigo criar_ataque(TipoAtaque tipo, int dano, const char *ataque_sprite, int hitbox, DirecaoAtaque direcao,SentidoAtaque sentido, int velocidade);
void inimigo_tomar_dano(Inimigo *inimigo, int dano);
void inimigo_tomar_mercy(Inimigo *inimigo, int dano);
//void limpar_inimigo(Inimigo *inimigo);

#endif