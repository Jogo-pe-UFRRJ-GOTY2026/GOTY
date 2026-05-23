#include "Inventario.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include "Atributos.h"

Arma criar_arma_Cap1(char nome[], char descricao[], int dano, int vida, TipoDano tipo);
Arma criar_arma_Cap2(char nome[], char descricao[], int dano, int vida, TipoDano tipo);
Arma criar_arma_Cap3(char nome[], char descricao[], int dano, int vida, TipoDano tipo);
Arma criar_arma_Cap4(char nome[], char descricao[], int dano, int vida, TipoDano tipo);

Inventario init_inventario()
{
    Inventario inventario;

    // ARMADURAS INICIAIS
    ParteArmadura peitoral, capacete, grevas, bota;
        
        capacete.atributosarmadura.defesa = 1;
        capacete.vida = 0;
        capacete.atributosarmadura.sorte = 0;
        capacete.atributosarmadura.forca = 0;
        strcpy(capacete.descricao, "Capacete simples e desgastado. Oferece pouca proteção, mas ainda pode salvar sua cabeça.");
        strcpy(capacete.nome, "Capacete Simples");

        peitoral.atributosarmadura.defesa = 2;
        peitoral.vida = 1;
        peitoral.atributosarmadura.sorte = 0;
        peitoral.atributosarmadura.forca = 0;
        strcpy(peitoral.descricao, "Peitoral velho feito de placas enferrujadas. Pesado, mas resistente o bastante para iniciantes.");
        strcpy(peitoral.nome, "Peitoral Velho");

        grevas.atributosarmadura.defesa = 1;
        grevas.vida = 0;
        grevas.atributosarmadura.sorte = 0;
        grevas.atributosarmadura.forca = 0;
        strcpy(grevas.descricao, "Grevas simples de couro endurecido. Quase sem conforto, mas ajudam na proteção.");
        strcpy(grevas.nome, "Grevas Simples");

        bota.atributosarmadura.defesa = 1;
        bota.vida = 0;
        bota.atributosarmadura.sorte = 0;
        bota.atributosarmadura.forca = 0;
        strcpy(bota.descricao, "Botas usadas com sola desgastada. Ainda servem para subir os andares da Torre.");
        strcpy(bota.nome, "Botas Gastas");

    
    inventario.armadura.capacete=capacete;
    inventario.armadura.peitoral=peitoral;
    inventario.armadura.grevas=grevas;
    inventario.armadura.botas=bota;

    //ARMA INICIAL
    Arma arma;
        strcpy(arma.nome, "Espada de Ferro Fraco");
        strcpy(arma.descricao, "Feita de ferro comum, sem reforço. Aguenta poucos golpes antes de perder o fio.");
        arma.vida = 0;
        arma.dano = 4;
        arma.tipo = CORTE;
        arma.atributosarma.defesa = 0; 
        arma.atributosarma.forca = 0;
        arma.atributosarma.sorte = 0;
    
        inventario.arma = arma;

    return inventario;
}

Arma criar_arma_Cap1(char nome[], char descricao[], int dano, int vida, TipoDano tipo)
{
    Arma arma;

    strcpy(arma.nome, nome);
    strcpy(arma.descricao, descricao);

    // Pequena variação para não ficar sempre igual
    arma.dano = dano + 1 + (rand() % 3); // +0 até +2

    arma.tipo = tipo;

    // Vida razoável para início de jogo
    arma.vida = vida + (rand() % 6); // +0 até +5

    // Atributos balanceados para Capítulo 1
    arma.atributosarma.defesa = rand() % 2; // 0 ou 1
    arma.atributosarma.forca = 1 + (rand() % 3); // 1 até 3
    arma.atributosarma.sorte = rand() % 2; // 0 ou 1

    return arma;
}

Arma criar_arma_Cap2(char nome[], char descricao[], int dano, int vida, TipoDano tipo)
{
    Arma arma;

    strcpy(arma.nome, nome);
    strcpy(arma.descricao, descricao);

    // Dano um pouco maior
    arma.dano = dano + 3 + (rand() % 4); // +3 até +6

    arma.tipo = tipo;

    // Mais Vida
    arma.vida = vida + 5 + (rand() % 8); // +5 até +12

    // Atributos intermediários
    arma.atributosarma.defesa = 1 + (rand() % 3); // 1 até 3
    arma.atributosarma.forca = 3 + (rand() % 4); // 3 até 6
    arma.atributosarma.sorte = rand() % 3; // 0 até 2

    return arma;
}

Arma criar_arma_Cap3(char nome[], char descricao[], int dano, int vida, TipoDano tipo)
{
    Arma arma;

    strcpy(arma.nome, nome);
    strcpy(arma.descricao, descricao);

    // Armas realmente fortes
    arma.dano = dano + 7 + (rand() % 6); // +7 até +12

    arma.tipo = tipo;

    // Bastante vida
    arma.vida = vida + 12 + (rand() % 12); // +12 até +23

    // Atributos avançados
    arma.atributosarma.defesa = 2 + (rand() % 4); // 2 até 5
    arma.atributosarma.forca = 6 + (rand() % 6); // 6 até 11
    arma.atributosarma.sorte = 1 + (rand() % 4); // 1 até 4

    return arma;
}

Arma criar_arma_Cap4(char nome[], char descricao[], int dano, int vida, TipoDano tipo)
{
    Arma arma;

    strcpy(arma.nome, nome);
    strcpy(arma.descricao, descricao);

    // Armas do fim do jogo
    arma.dano = dano + 12 + (rand() % 8); // +12 até +19

    arma.tipo = tipo;

    // Durabilidade muito alta
    arma.vida = vida + 20 + (rand() % 16); // +20 até +35

    // Atributos fortes
    arma.atributosarma.defesa = 4 + (rand() % 5); // 4 até 8
    arma.atributosarma.forca = 10 + (rand() % 8); // 10 até 17
    arma.atributosarma.sorte = 3 + (rand() % 5); // 3 até 7

    return arma;
}