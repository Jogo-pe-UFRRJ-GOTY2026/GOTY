#include "../system/Save.h"
#include "../system/Combate.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"



#define equals(str1, str2) (strcmp(str1, str2) == 0)

Inimigo *MockInimigo()
{

    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 10, "⟱▔", 4, HORIZONTAL),
        criar_ataque(BULLET, 2, "🗡", 1, VERTICAL),
        criar_ataque(BULLET, 3, "🟎", 1, VERTICAL)};

    Sprite_size size;
    size.x=43;
    size.y=28;

    Inimigo *inimigo = criar_inimigo(100, "Fallen King", "assets/sprites/bosses/fallen_king.txt",size, 40);

    for(int i=0;i<3;i++)
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

    player->inventario.arma = criar_arma("Espada de Teste", "Mocking", 999, CORTE);
    return player;
}


void simular_combate()
{
    Player *player=MockPlayer();
    Inimigo* inimigo=MockInimigo();
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
    assert(pl->inventario.arma.dano == 999);
    assert(pl->inventario.arma.tipo == CORTE);
    free(pl);
}

void test_saving_system()
{
    Player *mockedPlayer = MockPlayer();
    salvar_jogo_test(mockedPlayer);
    carregar_save_test();
}