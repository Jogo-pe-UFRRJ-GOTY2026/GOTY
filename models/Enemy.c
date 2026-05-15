#include "hashtable.h"
#include <stdlib.h>
#include "Enemy.h"

Enemy* enemy_create(int vida, int dano)
{
    Enemy* e = malloc(sizeof(Enemy));

    e->vida = vida;
    e->dano = dano;
    e->ataques = hashmap_create();

    return e;
}

void enemy_take_damage(Enemy* enemy, int dmg)
{
    enemy->vida -= dmg;

    if(enemy->vida < 0)
        enemy->vida = 0;
}

void enemy_destroy(Enemy* enemy)
{
    hashmap_destroy(enemy->ataques);

    free(enemy);
}