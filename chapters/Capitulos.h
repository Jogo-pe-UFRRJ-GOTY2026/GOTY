#ifndef CAPITULO_H
#define CAPITULO_H
#include "../objects/Player.h"
#include <stdbool.h>

Player* Prologo();
bool Capitulo1(Player *player, char tecla_pressionada);
bool Capitulo2(Player *player, char tecla_pressionada);
bool Capitulo3(Player *player, char tecla_pressionada);
bool Capitulo4(Player *player, char tecla_pressionada);

typedef enum Andar
{
    Andar0,
    Andar1,
    Andar2,
    Andar3,
    Andar4,
    Andar5,
    Andar6,
    Andar7,
    Andar8,
    Andar9,
    Andar10,
} Andar;

#endif