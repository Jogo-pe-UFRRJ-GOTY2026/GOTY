#include "Inventario.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "Atributos.h"

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

    inventario.armadura.capacete = capacete;
    inventario.armadura.peitoral = peitoral;
    inventario.armadura.grevas = grevas;
    inventario.armadura.botas = bota;

    // ARMA INICIAL
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

static const ArmaBase catalogo[] =
{
    // CORTE
    {
        "Espada Longa",
        "Uma espada equilibrada usada por aventureiros.", 
        CORTE, 6, 2, 
        "assets/sprites/loot/sword2.txt"
    },
    {
        "Espada Bastarda",
        "Uma lâmina robusta para combate prolongado.",
        CORTE, 7, 2,
        "assets/sprites/loot/sword1.txt"
    },

    {
        "Sabre Curvo",
        "Leve e rápido, ideal para ataques sucessivos.",
        CORTE, 5, 1,
        "assets/sprites/loot/sabre.txt"
    },
    {
        "Machado Real",
        "Um machado pesado semelhante ao usado pelo Hollow Knight.",
        CORTE, 8, 1,
        "assets/sprites/loot/axe.txt"
    },



    // PERFURAÇÃO
    {
        "Lança de Ferro",
        "Uma arma simples mas eficiente.",
        PERFURACAO, 7, 2,
        "assets/sprites/loot/spear.txt"
    },
    {
        "Tridente Infernal",
        "Três pontas capazes de atravessar armaduras.",
        PERFURACAO, 8, 2,
        "assets/sprites/loot/trident.txt"
    },
    {
        "Arco Longo",
        "Permite eliminar inimigos à distância.",
        PERFURACAO, 6, 1,
        "assets/sprites/loot/bow.txt"
    },

    {
        "Besta de Caça",
        "Dispara virotes com grande precisão.",
        PERFURACAO, 7, 1,
        "assets/sprites/loot/crossbow.txt"
    },



    // CONTUSÃO
    {
        "Martelo de Guerra",
        "Capaz de esmagar armaduras pesadas.",
        CONTUSAO,
        9,
        3,
        "assets/sprites/loot/hammer.txt"
    },
    {
        "Maça de Ferro",
        "Um bloco de metal preso a um cabo.",
        CONTUSAO, 10, 2,
        "assets/sprites/loot/mace.txt"
    },
    {
        "Escudo Torre",
        "Mais pesado que uma arma comum, mas confiável. Ofere proteção e permite golpear inimigos.",
        CONTUSAO, 5, 10, "assets/sprites/loot/shield1.txt"
    },
    {
        "Escudo Real",
        "Um grande escudo de aço semelhante ao usado pelo Hollow Knight. Ofere grande resistência ",
        CONTUSAO, 4, 12,
        "assets/sprites/loot/shield3.txt"
    },



    // MÁGICO
    {
        "Varinha Arcana",
        "Canaliza pequenas quantidades de energia mágica.",
        MAGICO, 6, 2,
        "assets/sprites/loot/magic_wand.txt"
    },
    {
        "Varinha Rúnica",
        "Runas antigas brilham em sua superfície.",
        MAGICO, 7, 3,
        "assets/sprites/loot/magic_wand2.txt"
    },

    {
        "Cajado Mítico",
        "Um cajado com estrutura de aço engravado em runas máginas.",
        MAGICO, 7, 3,
        "assets/sprites/loot/magic_staff2.txt"
    }
};



Arma gerar_arma_aleatoria(int capitulo)
{
    int indice = rand() % (sizeof(catalogo) / sizeof(catalogo[0]));

    ArmaBase base = catalogo[indice];

    Arma arma =
        criar_arma(
            capitulo,
            base.nome,
            base.descricao,
            base.danoBase,
            base.vidaBase,
            base.tipo
        );

    strcpy(arma.sprite, base.sprite);

    return arma;
}








Arma criar_arma(int capitulo, char nome[], char descricao[], int danoBase, int vidaBase, TipoDano tipo)
{

    Arma arma = gerar_arma_aleatoria(capitulo);
    
    int bonusDano = 0;
    int bonusForca = 0;
    int bonusDefesa = 0;
    int bonusSorte = 0;

    // DIFERENÇA ENTRE OS TIPOS DE DANO
    switch(tipo)
    {
        case CORTE:
            bonusDano = 2;
            bonusForca = 1;
            break;

        case PERFURACAO:
            bonusDano = 1;
            bonusSorte = 2;
            break;

        case CONTUSAO:
            bonusDefesa = 2;
            bonusForca = 2;
            break;

        case MAGICO:
            bonusSorte = 3;
            bonusDano = 3;
            break;
    }

    // ESCALONAMENTO POR CAPÍTULO
    switch(capitulo)
    {
        case 1:
            arma.dano = danoBase + bonusDano + (rand() % 3);
            arma.vida = vidaBase + (rand() % 4);
            arma.atributosarma.defesa = bonusDefesa + (rand() % 2);
            arma.atributosarma.forca = bonusForca + (rand() % 3);
            arma.atributosarma.sorte = bonusSorte + (rand() % 2);
            break;

        case 2:
            arma.dano = danoBase + 4 + bonusDano + (rand() % 4);
            arma.vida = vidaBase + 6 + (rand() % 6);
            arma.atributosarma.defesa = 1 + bonusDefesa + (rand() % 3);
            arma.atributosarma.forca = 2 + bonusForca + (rand() % 4);
            arma.atributosarma.sorte = bonusSorte + (rand() % 3);
            break;

        case 3:
            arma.dano = danoBase + 10 + bonusDano + (rand() % 6);
            arma.vida = vidaBase + 14 + (rand() % 8);
            arma.atributosarma.defesa = 3 + bonusDefesa + (rand() % 4);
            arma.atributosarma.forca = 5 + bonusForca + (rand() % 5);
            arma.atributosarma.sorte = 1 + bonusSorte + (rand() % 4);
            break;
        case 4:
            arma.dano = danoBase + 18 + bonusDano + (rand() % 8);
            arma.vida = vidaBase + 22 + (rand() % 12);
            arma.atributosarma.defesa = 5 + bonusDefesa + (rand() % 5);
            arma.atributosarma.forca = 8 + bonusForca + (rand() % 6);
            arma.atributosarma.sorte = 3 + bonusSorte + (rand() % 5);
            break;
    }

    return arma;
}

ParteArmadura criar_armadura(int capitulo, TipoArmadura tipo, char nome[],char descricao[], int vidaBase)
{
    ParteArmadura armadura;

    strcpy(armadura.nome, nome);
    strcpy(armadura.descricao, descricao);

    int bonusDefesa = 0;
    int bonusVida = 0;
    int bonusForca = 0;
    int bonusSorte = 0;

    // DIFERENÇA ENTRE AS PARTES

    switch(tipo)
    {
        case CAPACETE:
            bonusDefesa = 1;
            bonusSorte = 1;
            break;

        case PEITORAL:
            bonusDefesa = 3;
            bonusVida = 5;
            break;

        case GREVAS:
            bonusDefesa = 2;
            bonusVida = 2;
            break;

        case BOTAS:
            bonusDefesa = 1;
            bonusSorte = 2;
            break;
    }

    // ESCALONAMENTO POR CAPÍTULO

    switch(capitulo)
    {
        case 1:
            armadura.vida = vidaBase + bonusVida + (rand() % 3);
            armadura.atributosarmadura.defesa = bonusDefesa + (rand() % 2);
            armadura.atributosarmadura.forca = rand() % 2;
            armadura.atributosarmadura.sorte = bonusSorte + (rand() % 2);
            break;

        case 2:
            armadura.vida = vidaBase + 5 + bonusVida + (rand() % 5);
            armadura.atributosarmadura.defesa = 2 + bonusDefesa + (rand() % 3);
            armadura.atributosarmadura.forca = 1 + (rand() % 3);
            armadura.atributosarmadura.sorte = bonusSorte + (rand() % 3);
            break;

        case 3:
            armadura.vida = vidaBase + 12 + bonusVida + (rand() % 8);
            armadura.atributosarmadura.defesa = 5 + bonusDefesa + (rand() % 4);
            armadura.atributosarmadura.forca = 3 + (rand() % 4);
            armadura.atributosarmadura.sorte = 1 + bonusSorte + (rand() % 3);
            break;

        case 4:
            armadura.vida = vidaBase + 20 + bonusVida + (rand() % 12);
            armadura.atributosarmadura.defesa = 8 + bonusDefesa + (rand() % 5);
            armadura.atributosarmadura.forca = 5 + (rand() % 5);
            armadura.atributosarmadura.sorte = 2 + bonusSorte + (rand() % 5);
            break;
    }

    return armadura;
}


