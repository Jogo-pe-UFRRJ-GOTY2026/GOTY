#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <ncurses.h>
#include "../objects/Player.h"

typedef enum opcao_selecionada
{
    INICIAR_JOGO_NOVO,
    CONTINUAR_JOGO,
    ABRIR_CONFIGURACOES,
    FECHAR_JOGO,
    TOTAL_OPCOES
} opcao_selecionada;

typedef enum cores
{
    COR_OPCAO_SELECIONADA = 1,
    COR_VIDA,
    COR_NOME_BOSS,
    COR_TEXTO_MORTE,
    COR_DESTAQUE,
} Cores;


typedef enum Exceptions
{
    Exception_AllocationError = 1,
    Exception_KeyError,
    Exception_NotImplementedError,
    Exception_IndexError,
    Exception_OverflowError,
    Exception_FileReadWriteError
} Exceptions;




void desenhar_sprite(WINDOW* win, const char* nome_arquivo, int y_inicial, int x_inicial);
int escolher_ataque();
int gerar_rolagem_dano();
int gerar_chance_de_evasao_do_inimigo();
int gerar_rolagem_mercy();
void exit_with_error(Exceptions ex);
void slow_mvwprintw(WINDOW *win, char *fala, const int y, int x, int delay_ms);
void mostrar_tela_morte(Player* player);

void iniciar_cores();

#define len(arr) (sizeof(arr)/sizeof(arr[0]))

#endif