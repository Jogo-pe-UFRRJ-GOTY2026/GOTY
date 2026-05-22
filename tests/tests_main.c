#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../objects/Player.h"
#include "../system/Save.h"

// Uma breve aula para vocês senhores, um hábito muito bom de se criar em desenvolvimento de sistemas: Testes
// É prática existente na industria escrever testes para o seu sistema, para previnir erros durante as suas pipelines CI/CD e deploys incorretos
// Testes normalmente podem ser separados em alguns grupos, em ordem de custo e trabalho de implementar:
// 1. Unit Tests: Testa componentes individuais do codigo. Em python há bibliotecas como pytest e unittest que facilitam isso,
// No springboot do java há anotações que facilitam isso como @SpringBootTest @AutoConfigureMockMvc @Test
// 2. Integration Test: Testam o comportamento e comunicação dos componentes e serviços do sistema trabalhando juntos
// 3. System Testing: verifica se o sistema inteiro ta funcional
// 4. End to end testing -> Ignoramos esse até que seja absolutamente


#define equals(str1, str2) (strcmp(str1,str2)==0)

// Declara prototipos das funções
Player *MockPlayer();
void salvar_jogo_test(Player *player);
void carregar_save_test();
void test_saving_system();





int main()
{
    test_saving_system();
    printf("Teste do sistema de salvamento passados com sucesso\n");

    return 0;
}

Player *MockPlayer()
{
    Player *player = criar_player("Kiara", Feminino);
    player->level = 10;
    player->vida = 87;
    player->karma = 25;
    player->posicao.x = 0;
    player->posicao.y = 0;

    player->NumeroAndar = 3;

    player->inventario.arma = criar_arma("Espada de Teste", "Mocking", 999, CORTE);
    return player;
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