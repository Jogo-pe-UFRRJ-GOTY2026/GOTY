#ifndef PLAYER_H
#define PLAYER_H
#include <stddef.h>
#include "Inventario.h"
#include "Ataque.h"

typedef enum Classe{
    Mago, 
    Guerreiro,
    Arqueiro,

} Classe;

typedef enum Genero
{
    Masculino,
    Feminino,
    Outro
} Genero;

typedef struct Atributos{
    int vida;
    int forca;
    int agilidade;
    int inteligencia;
    int vitalidade;
} Atributos;

typedef struct Position{
    int x;
    int y;
} Position;

typedef struct Player{
    char nome[100];
    int level;
    int defesa;
    int karma;
    Genero genero;
    Inventario inventario;
    Position posicao;
    Atributos atributos;
    size_t NumeroAndar;
} Player;

void mover(Player *player, Inimigo *inimigo);
void tomar_dano(Player *player, AtaqueInimigo *atack);
void atacar_inimigo(Player *player, Inimigo *inimigo);

#endif