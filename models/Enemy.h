#ifndef ENEMY_H
#define ENEMY_H

#include "../data_structures/hashtable.h"


typedef struct Enemy
{
    int vida;
    int dano;
    hash_table* ataques;

} Enemy;

Enemy* enemy_create(int vida, int dano);

void enemy_take_damage(Enemy* enemy, int dmg);

void enemy_destroy(Enemy* enemy);

#endif