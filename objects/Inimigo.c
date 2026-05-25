#include "Inimigo.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

AtaqueInimigo criar_ataque(TipoAtaque tipo, int dano, const char* ataque_sprite, int hitbox, DirecaoAtaque direcao,SentidoAtaque sentido, int velocidade)
{
    AtaqueInimigo ataque;

    ataque.tipo_ataque=tipo;
    ataque.dano=dano;
    ataque.hit_box=hitbox;
    //printf("\n\n%s\n\n", ataque_sprite);
    strncpy(ataque.ataque_sprite,ataque_sprite, sizeof(ataque.ataque_sprite)-1);
    ataque.direcao=direcao;
    ataque.sentido=sentido;
    ataque.ativo=false;
    ataque.velocidade = velocidade;
    ataque.tick_movimento = 0;
    ataque.tick_vida = 0;
    return ataque;
}

Inimigo *criar_inimigo(int vida, char nome[], const char *sprite_location, Sprite_size size, int mercy)
{
    Inimigo *inimigo = malloc(sizeof(Inimigo));
    if(inimigo==NULL)
        perror("Foda ne cria");

    strncpy(inimigo->nome, nome, sizeof(inimigo->nome));
    inimigo->vida = vida;
    inimigo->mercy = mercy;
    inimigo->sprite_size.x=size.x;
    inimigo->sprite_size.y=size.y;
    strncpy(inimigo->sprite, sprite_location, sizeof(inimigo->sprite));
    return inimigo;
}

void inimigo_tomar_dano(Inimigo* inimigo, int dano)
{
    inimigo->vida -= dano;

    if (inimigo->vida < 0)
        inimigo->vida = 0;
}

