#ifndef COMBAT_H
#define COMBAT_H

#include "Player.h"
#include "Inimigo.h"
#include <ncurses.h>
#include <stdbool.h>

// ── Dimensões do campo de batalha ─────────────────────────────────────
// Cada célula ocupa 2 colunas no terminal (proporção mais quadrada).
// O campo total renderizado = CAMPO_COLS * 2 + 2 (bordas) ≤ WIN_W.
#define CAMPO_COLS 34   // colunas lógicas
#define CAMPO_ROWS 12   // linhas lógicas

// ── Layout das janelas ncurses ─────────────────────────────────────────
// Terminal mínimo recomendado: 80 colunas × 50 linhas
#define WIN_X         3    // margem esquerda de todas as janelas
#define WIN_W        74    // largura de todas as janelas

#define INI_WIN_Y     0    // janela do inimigo: linha inicial
#define INI_WIN_H    24    // altura (inclui bordas e arte ASCII)

#define CAM_WIN_Y    24    // janela do campo: logo abaixo da janela do inimigo
#define CAM_WIN_H    16    // altura (inclui bordas e 1 linha de mensagem)

#define STATUS_ROW   40    // linha da barra de status HP / LV
#define MENU_WIN_Y   42    // janela do menu de ações
#define MENU_WIN_H    7    // altura (2 linhas de botões × 3 linhas cada + borda)

// ── Pares de cor ──────────────────────────────────────────────────────
#define COR_NORMAL   1    // branco no preto  — texto padrão
#define COR_HP       2    // vermelho no preto — vida / dano
#define COR_SEL      3    // preto no amarelo  — botão selecionado
#define COR_HEART    4    // vermelho no preto — coração do player (♥)
#define COR_PERIGO   5    // branco no vermelho — zona de ataque do inimigo
#define COR_INI_NAME 6    // ciano no preto    — nome do inimigo

// ── Ações disponíveis no menu ─────────────────────────────────────────
typedef enum AcaoCombate {
    ACAO_ATACAR   = 0,   // posição (0,0) do grid 2×2
    ACAO_DESISTIR = 1,   // posição (0,1)
    ACAO_ITEM     = 2,   // posição (1,0)
    ACAO_MERCY    = 3,   // posição (1,1)
    ACAO_COUNT    = 4,
} AcaoCombate;

// ── Estado de saída do combate ────────────────────────────────────────
typedef enum EstadoCombate {
    COMBATE_ATIVO,       // rodada em andamento
    COMBATE_VITORIA,     // inimigo derrotado
    COMBATE_DERROTA,     // player morreu
    COMBATE_FUGA,        // player escolheu DESISTIR
    COMBATE_MERCY_OK,    // inimigo poupado
} EstadoCombate;

// ── Contexto principal de combate ─────────────────────────────────────
typedef struct CombateCtx {
    Player        *player;
    Inimigo       *inimigo;

    int            player_vida_max;    // vida inicial do player (para barra de HP)
    int            inimigo_vida_max;   // vida inicial do inimigo (para barra de HP)

    AcaoCombate    acao_selecionada;
    EstadoCombate  estado;
    char           mensagem[256];      // mensagem exibida na janela do inimigo

    // Posição do ♥ dentro do campo lógico (coluna, linha)
    int            px;
    int            py;

    // Janelas ncurses
    WINDOW        *win_inimigo;        // arte + nome + HP do inimigo + mensagem
    WINDOW        *win_campo;          // campo de esquiva
    WINDOW        *win_menu;           // menu 2×2 de ações
} CombateCtx;

// ── API pública ────────────────────────────────────────────────────────

/**
 * Aloca e inicializa o contexto de combate.
 * Cria as janelas ncurses (initscr() deve ter sido chamado antes).
 * @param player   Ponteiro para o Player participante.
 * @param inimigo  Ponteiro para o Inimigo participante.
 * @return Ponteiro para o CombateCtx alocado, ou NULL em falha.
 */
CombateCtx    *combate_init(Player *player, Inimigo *inimigo);

/**
 * Executa o loop principal de combate até que uma condição de término
 * seja atingida (vitória, derrota, fuga ou mercy).
 * @param ctx Contexto de combate previamente inicializado.
 * @return Estado final do combate.
 */
EstadoCombate  combate_loop(CombateCtx *ctx);

/**
 * Libera todos os recursos alocados pelo contexto (janelas + struct).
 * @param ctx Contexto de combate a ser liberado.
 */
void           combate_cleanup(CombateCtx *ctx);

#endif // COMBAT_H
