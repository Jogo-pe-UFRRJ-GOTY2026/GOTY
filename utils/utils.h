#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

typedef enum opcao_selecionada
{
    INICIAR_JOGO_NOVO,
    CONTINUAR_JOGO,
    ABRIR_CONFIGURACOES,
    FECHAR_JOGO,
    TOTAL_OPCOES
} opcao_selecionada;

typedef enum{
    COR_OPCAO_SELECIONADA=1,
    COR_VIDA,
    COR_NOME,
} Cores;
typedef enum Exceptions
{
    Exception_AllocationError = 1,
    Exception_KeyError,
    Exception_NotImplementedError,
    Exception_IndexError,
    Exception_OverflowError
} Exceptions;

int gerar_rolagem_dano();
int gerar_chance_de_evasao_do_inimigo();
void exit_with_error(Exceptions ex);

typedef enum Direcao {
    ESQUERDA,
    DIREITA,
    BAIXO,
    CIMA,
    DIRECAO_INVALIDA
} Direcao;

void iniciar_cores();

#define len(arr) (sizeof(arr)/sizeof(arr[0]))

#endif