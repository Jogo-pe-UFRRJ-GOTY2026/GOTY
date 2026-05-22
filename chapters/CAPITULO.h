#ifndef CAPITULO_H
#define CAPITULO_H

#include "../objects/Player.h"
#include <stdbool.h>

Player* Prologo();
Player *Tela_criacao();

bool Capitulo1(Player *player);
bool Capitulo2(Player *player);
bool Capitulo3(Player *player);
bool Capitulo4(Player *player);

typedef enum
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
    Andar10
} Andar;

#endif