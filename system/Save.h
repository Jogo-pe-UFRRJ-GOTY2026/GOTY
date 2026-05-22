#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>
#include "../objects/Player.h"


Player* carregar_salvamento();
bool salvar_jogo(Player* player);

#endif