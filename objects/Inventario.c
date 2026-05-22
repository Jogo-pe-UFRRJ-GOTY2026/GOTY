#include "Inventario.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>


Inventario init_inventario()
{
    Inventario inventario;


    ParteArmadura peitoral, capacete, grevas, bota;
        
        capacete.defesa = 1;
        strcpy(capacete.descricao, "Capacete simples e desgastado. Oferece pouca proteção, mas ainda pode salvar sua cabeça.");
        strcpy(capacete.nome, "Capacete Simples");

        peitoral.defesa = 2;
        strcpy(peitoral.descricao, "Peitoral velho feito de placas enferrujadas. Pesado, mas resistente o bastante para iniciantes.");
        strcpy(peitoral.nome, "Peitoral Velho");

        grevas.defesa = 1;
        strcpy(grevas.descricao, "Grevas simples de couro endurecido. Quase sem conforto, mas ajudam na proteção.");
        strcpy(grevas.nome, "Grevas Simples");

        bota.defesa = 1;
        strcpy(bota.descricao, "Botas usadas com sola desgastada. Ainda servem para subir os andares da Torre.");
        strcpy(bota.nome, "Botas Gastas");

    
    inventario.armadura.capacete=capacete;
    inventario.armadura.peitoral=peitoral;
    inventario.armadura.grevas=grevas;
    inventario.armadura.botas=bota;


    inventario.arma = criar_arma("Espada de Ferro Fraco", "Feita de ferro comum, sem reforço. Aguenta poucos golpes antes de perder o fio.", 4, CORTE);
    


    return inventario;
}

Arma criar_arma(char nome[], char descricao[], int dano, TipoDano tipo)
{
    Arma arma;


    strcpy(arma.nome, nome);
    strcpy(arma.descricao, descricao);
    arma.dano = dano;
    arma.tipo = tipo;

    return arma;
}

