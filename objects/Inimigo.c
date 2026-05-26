#include "Inimigo.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#define equals(str1, str2) (strcmp(str1, str2) == 0)

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

Inimigo *criar_inimigo(int vida, char nome[], const char *sprite_location, Sprite_size size, int mercy, int tempo_por_rodada)
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
    inimigo->tempo_por_rodada=tempo_por_rodada;
    return inimigo;
}

void inimigo_tomar_dano(Inimigo* inimigo, int dano)
{
    inimigo->vida -= dano;

    if (inimigo->vida < 0)
        inimigo->vida = 0;
}

void renderizar_nome_estilizado(WINDOW *area_nome_boss, const char *nome)
{
    int meio = (getmaxx(area_nome_boss) / 2) - 10 - strlen(nome);
    wattron(area_nome_boss, COLOR_PAIR(COR_NOME_BOSS) | A_BOLD);

    if (equals("Fallen King", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "██████ ▄▄▄  ▄▄    ▄▄    ▄▄▄▄▄ ▄▄  ▄▄   ██ ▄█▀ ▄▄ ▄▄  ▄▄  ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██▄▄  ██▀██ ██    ██    ██▄▄  ███▄██   ████   ██ ███▄██ ██ ▄▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "██    ██▀██ ██▄▄▄ ██▄▄▄ ██▄▄▄ ██ ▀██   ██ ▀█▄ ██ ██ ▀██ ▀███▀", nome);
    }
    if (equals("Hollow Knight", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "██  ██  ▄▄▄  ▄▄    ▄▄     ▄▄▄  ▄▄   ▄▄   ██ ▄█▀ ▄▄  ▄▄ ▄▄  ▄▄▄▄ ▄▄ ▄▄ ▄▄▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██████ ██▀██ ██    ██    ██▀██ ██ ▄ ██   ████   ███▄██ ██ ██ ▄▄ ██▄██   ██   ", nome);
        mvwprintw(area_nome_boss, 2, meio, "██  ██ ▀███▀ ██▄▄▄ ██▄▄▄ ▀███▀  ▀█▀█▀    ██ ▀█▄ ██ ▀██ ██ ▀███▀ ██ ██   ██   ", nome);
    }
    if (equals("Cerberus", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "▄█████ ▄▄▄▄▄ ▄▄▄▄  ▄▄▄▄  ▄▄▄▄▄ ▄▄▄▄  ▄▄ ▄▄  ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██     ██▄▄  ██▄█▄ ██▄██ ██▄▄  ██▄█▄ ██ ██ ███▄▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "▀█████ ██▄▄▄ ██ ██ ██▄█▀ ██▄▄▄ ██ ██ ▀███▀ ▄▄██▀ ", nome);
    }
    if (equals("Demon", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "████▄  ▄▄▄▄▄ ▄▄   ▄▄  ▄▄▄  ▄▄  ▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██  ██ ██▄▄  ██▀▄▀██ ██▀██ ███▄██", nome);
        mvwprintw(area_nome_boss, 2, meio, "████▀  ██▄▄▄ ██   ██ ▀███▀ ██ ▀██", nome);
    }
    if (equals("Dragon", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "████▄  ▄▄▄▄   ▄▄▄   ▄▄▄▄  ▄▄▄  ▄▄  ▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██  ██ ██▄█▄ ██▀██ ██ ▄▄ ██▀██ ███▄██", nome);
        mvwprintw(area_nome_boss, 2, meio, "████▀  ██ ██ ██▀██ ▀███▀ ▀███▀ ██ ▀██", nome);
    }
    if (equals("Centaur", nome))
    {
        mvwprintw(area_nome_boss, 0, meio, "▄█████ ▄▄▄▄▄ ▄▄  ▄▄ ▄▄▄▄▄▄ ▄▄▄  ▄▄ ▄▄ ▄▄▄▄", nome);
        mvwprintw(area_nome_boss, 1, meio, "██     ██▄▄  ███▄██   ██  ██▀██ ██ ██ ██▄█▄", nome);
        mvwprintw(area_nome_boss, 2, meio, "▀█████ ██▄▄▄ ██ ▀██   ██  ██▀██ ▀███▀ ██ ██", nome);
    }
    wattroff(area_nome_boss, COLOR_PAIR(COR_NOME_BOSS) | A_BOLD);
}