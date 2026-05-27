#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "CAPITULO.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"
#include "../system/Combate.h"
#include "../utils/utils.h"


void Prologo_pt2(Player* player)
{

    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 10, "█▙▔▔▔", 1, HORIZONTAL, DIREITA_ESQUERDA, 2),
        criar_ataque(BULLET, 2, "†", 1, VERTICAL, CIMA_BAIXO, 3),
        criar_ataque(BULLET, 3, "*", 1, HORIZONTAL, ESQUERDA_DIREITA, 5)};

    Sprite_size size;
    size.x=33;
    size.y=20;

    Inimigo *hollow_knigth = criar_inimigo(100, "Hollow Knight", "assets/sprites/bosses/hollow.txt",size, 40, 20);

    hollow_knigth->numero_ataques=len(ataques);
    for(int i=0;i<hollow_knigth->numero_ataques;i++)
    {
        hollow_knigth->ataques[i] = ataques[i];
    }

    iniciar_combate(player, hollow_knigth);



}