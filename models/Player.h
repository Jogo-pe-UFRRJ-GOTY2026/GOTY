#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player{
    char nome[100];
    int level;
    int vida;
    int x;
    int y;
    // Armadura
    // Armas
    // Consumiveis
} Player;

void move(int x, int y);
void surrender();
void atack_enemy(int damage);

#endif