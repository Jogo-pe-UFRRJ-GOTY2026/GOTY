#include "../system/Save.h"
#include "../system/Combate.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"

#define equals(str1, str2) (strcmp(str1, str2) == 0)

Inimigo *MockInimigo()
{
    //exemplo da criação ataques
    AtaqueInimigo ataques[] = {
        criar_ataque(LASER, 10, "🞕", 0, HORIZONTAL, NAO_IMPORTA, 90, 60),
        criar_ataque_area(10, "^", 0, 4, 4, 90, 90),
        criar_ataque(PAREDE, 2, "█", 0, VERTICAL, DIREITA_ESQUERDA, 10, 180),
    };

    Sprite_size size;
    size.x=19;
    size.y=21;

    Inimigo *inimigo = criar_inimigo(100, "Demon", "assets/sprites/bosses/demon.txt",size, 40, 30, Cruciator);

    inimigo->numero_ataques=len(ataques);
    for(int i=0;i<inimigo->numero_ataques;i++)
    {
        inimigo->ataques[i] = ataques[i];
    }

    return inimigo;
}

Player *MockPlayer()
{
    Player *player = criar_player("Kiara", Feminino);
    player->level = 10;
    player->vida = 87;
    player->vida_max=87;
    player->karma = 25;
    player->posicao.x = 0;
    player->posicao.y = 0;

    player->NumeroAndar = 3;

    player->inventario.arma = criar_arma(1, "Espada de Teste", "Mocking", 40, 0, CORTE);
    return player;
}

void simular_combate()
{
    Player *player=MockPlayer();
    Inimigo* inimigo=MockInimigo();
    //mostrar_tela_morte(player);
    
    iniciar_combate(player, inimigo);
}



void salvar_jogo_test(Player *player)
{
    bool salvou = salvar_jogo(player);
    free(player);
    assert(salvou == true);
}

void carregar_save_test()
{
    Player *pl = carregar_salvamento();
    assert(equals(pl->nome, "Kiara"));
    assert(pl->genero == Feminino);
    assert(pl->vida == 87);
    assert(pl->karma == 25);
    assert(pl->NumeroAndar == 3);

    assert(equals(pl->inventario.arma.nome, "Espada de Teste"));
    assert(equals(pl->inventario.arma.descricao, "Mocking"));
    assert(pl->inventario.arma.tipo == CORTE);
    free(pl);
}

void test_saving_system()
{
    Player *mockedPlayer = MockPlayer();
    salvar_jogo_test(mockedPlayer);
    carregar_save_test();
}