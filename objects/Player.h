#ifndef PLAYER_H
#define PLAYER_H
#include <stddef.h>
#include <stdbool.h>
#include <ncurses.h>
#include "Inventario.h"
#include "Inimigo.h"
#include "../utils/utils.h"


typedef enum Genero
{
    Masculino,
    Feminino
} Genero;

// typedef struct Atributos{
//     int forca;
//     int agilidade;
//     int inteligencia;
//     int vitalidade;
// } Atributos;

typedef struct DeltaDirecao
{
    int dx;
    int dy;
} DeltaDirecao;

typedef struct Position{
    int x;
    int y;
} Position;

typedef struct Player{
    char nome[50];
    int level;
    int vida_max;
    int vida;
    int defesa;
    int karma;
    Genero genero;
    Inventario inventario;
    Position posicao;
    // Atributos atributos;
    size_t NumeroAndar;
} Player;


Player *criar_player(const char *nome, Genero genero);
void mover_player(Player *player, DeltaDirecao direcao, int limite_esquerda, int limite_direita, int limite_cima, int limite_baixo);
void tomar_dano(Player *player, AtaqueInimigo *atack);
bool atacar_inimigo(Player *player, Inimigo *inimigo);
DeltaDirecao get_delta_direcao(WINDOW *win);

#endif