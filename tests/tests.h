#ifndef TEST_H
#define TEST_H

#include "../objects/Player.h"



// Declara prototipos das funções
Player *MockPlayer();
Inimigo *MockInimigo();
void simular_combate();

void salvar_jogo_test(Player *player);
void carregar_save_test();
void test_saving_system();

#endif