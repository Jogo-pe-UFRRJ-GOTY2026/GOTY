/*
 * combat.c — Sistema de Combate (ncurses)
 *
 * Fluxo de uma rodada:
 *   1. Menu: player navega com setas e confirma com Enter.
 *   2. Ação: ATACAR / ITEM / MERCY / DESISTIR.
 *   3. Turno do inimigo: fase de esquiva em tempo real.
 *      - Player move ♥ com setas para desviar das zonas de perigo (██).
 *      - Ao fim do timer, verifica colisão e aplica dano.
 *
 * Dependências de compilação (adicione ao Makefile):
 *   -lncurses -lm
 *   Se usar UTF-8 (♥ █): compile com -lncursesw e inclua setlocale.
 *
 * Assumido da ArrayList:
 *   int   arraylist_size(ArrayList *list);
 *   void *arraylist_get (ArrayList *list, int index);
 */

#include "Combate_claude_example.h"
#include "Combate.h"
#include "../utils/utils.h"
#include "../data_structures/arraylist.h"
#include "../objects/Player.h"

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// ── Configuração da fase de esquiva ───────────────────────────────────
#define ESQUIVA_FRAMES   75      // número de frames totais de esquivasystem/Combate_claude_example.c
#define ESQUIVA_FRAME_US 66667   // duração de cada frame em µs (~15 fps)

// ── Labels do menu 2×2 ────────────────────────────────────────────────
static const char *MENU_LABEL[ACAO_COUNT] = {
    "    ATACAR    ",
    "  DESISTIR    ",
    "     ITEM     ",
    "    MERCY     ",
};

// ── Arte ASCII padrão do inimigo ──────────────────────────────────────
// Exibida centralizada na janela superior.
static const char *INIMIGO_ART[] = {
    "         ,                           ",
    "        / \\, | ,        .--.         ",
    "       |    =|= >      /.--.\\.       ",
    "        \\ /` | `      |====|         ",
    "         `   |         |`::`|         ",
    "             |    .-;`\\..../`;_.-^-._ ",
    "            /\\\\/  /  |...::..|`  :  `|",
    "            |:'\\ |   /'''::''|  .:.  |",
    "             \\ /\\;-,/\\   ::  |.......|",
    "             |\\ <` >  >._::_.| ':::' |",
    "             | `\"\"`  /   ^^  |  ':'  |",
    "             |       |       \\   :   /",
    "             |       |        \\  :  / ",
    "             |       |___/\\___|`-.-`  ",
    "             |        \\_ || _/        ",
    "             |        <_ >< _>        ",
    "             |        |  ||  |        ",
    "             |       _\\.:||:./_       ",
    "             |      /____/\\____\\      ",
};
#define N_ART_LINES ((int)(sizeof(INIMIGO_ART) / sizeof(INIMIGO_ART[0])))

// ═════════════════════════════════════════════════════════════════════
// Funções de renderização (estáticas — internas ao módulo)
// ═════════════════════════════════════════════════════════════════════

static void init_cores(void)
{
    if (!has_colors()) return;
    start_color();
    use_default_colors();
    init_pair(COR_NORMAL,   COLOR_WHITE,  COLOR_BLACK);
    init_pair(COR_HP,       COLOR_RED,    COLOR_BLACK);
    init_pair(COR_SEL,      COLOR_BLACK,  COLOR_YELLOW);
    init_pair(COR_HEART,    COLOR_RED,    COLOR_BLACK);
    init_pair(COR_PERIGO,   COLOR_WHITE,  COLOR_RED);
    init_pair(COR_INI_NAME, COLOR_CYAN,   COLOR_BLACK);
}

/* Desenha a barra de HP na linha `row` da janela `w`.
 * Formato:  HP: cur / max  [████████░░░░]
 */
static void draw_hp_bar(WINDOW *w, int row, int col,
                         int cur, int max, int bar_w)
{
    if (max <= 0) max = 1;
    int filled = cur * bar_w / max;
    if (filled < 0)      filled = 0;
    if (filled > bar_w)  filled = bar_w;

    wattron(w, COLOR_PAIR(COR_HP) | A_BOLD);
    mvwprintw(w, row, col, "HP: %3d / %3d [", cur, max);
    wattroff(w, COLOR_PAIR(COR_HP) | A_BOLD);

    wattron(w, COLOR_PAIR(COR_HP) | A_REVERSE);
    for (int i = 0; i < filled; i++) waddch(w, ' ');
    wattroff(w, COLOR_PAIR(COR_HP) | A_REVERSE);

    wattron(w, COLOR_PAIR(COR_NORMAL));
    for (int i = filled; i < bar_w; i++) waddch(w, '-');
    waddch(w, ']');
    wattroff(w, COLOR_PAIR(COR_NORMAL));
}

/* Janela superior: arte ASCII do inimigo, nome, barra de HP e mensagem. */
static void draw_inimigo(CombateCtx *ctx)
{
    WINDOW *w = ctx->win_inimigo;
    werase(w);
    box(w, 0, 0);

    // ── Nome do inimigo (centralizado, ciano, negrito) ────────────────
    int name_len = (int)strlen(ctx->inimigo->nome);
    int name_col = (WIN_W - name_len) / 2;
    if (name_col < 1) name_col = 1;

    wattron(w, COLOR_PAIR(COR_INI_NAME) | A_BOLD);
    mvwprintw(w, 1, name_col, "%s", ctx->inimigo->nome);
    wattroff(w, COLOR_PAIR(COR_INI_NAME) | A_BOLD);

    // ── Arte ASCII (centralizada verticalmente entre linhas 2 e INI_WIN_H-4) ──
    int art_area = INI_WIN_H - 5;   // área disponível (excluindo bordas e status)
    int art_start_y = 2 + (art_area - N_ART_LINES) / 2;
    if (art_start_y < 2) art_start_y = 2;

    for (int i = 0; i < N_ART_LINES; i++) {
        int y = art_start_y + i;
        if (y >= INI_WIN_H - 3) break;

        int art_len = (int)strlen(INIMIGO_ART[i]);
        int art_col = (WIN_W - art_len) / 2;
        if (art_col < 1) art_col = 1;

        mvwprintw(w, y, art_col, "%s", INIMIGO_ART[i]);
    }

    // ── Barra de HP do inimigo ────────────────────────────────────────
    draw_hp_bar(w, INI_WIN_H - 3, 2,
                ctx->inimigo->vida, ctx->inimigo_vida_max,
                WIN_W - 22);

    // ── Mensagem de combate (linha inferior, dentro da borda) ─────────
    if (ctx->mensagem[0]) {
        wattron(w, COLOR_PAIR(COR_NORMAL) | A_ITALIC);
        mvwprintw(w, INI_WIN_H - 2, 2, "%-*.*s",
                  WIN_W - 3, WIN_W - 3, ctx->mensagem);
        wattroff(w, COLOR_PAIR(COR_NORMAL) | A_ITALIC);
    }

    wrefresh(w);
}

/* Constrói o mapa de zonas de perigo conforme o TipoAtaque.
 * danger[row][col] = true  →  célula ativa (ataque do inimigo)
 */
static void build_danger_map(bool danger[CAMPO_ROWS][CAMPO_COLS],
                              const AtaqueInimigo *atk)
{
    memset(danger, 0, sizeof(bool) * CAMPO_ROWS * CAMPO_COLS);

    int cx = ((atk->x % CAMPO_COLS) + CAMPO_COLS) % CAMPO_COLS;
    int cy = ((atk->y % CAMPO_ROWS) + CAMPO_ROWS) % CAMPO_ROWS;
    int r  = atk->hit_box > 0 ? atk->hit_box : 1;

    switch (atk->tipo_ataque)
    {
        case LINHA:
            // Linha horizontal inteira na linha cy
            for (int c = 0; c < CAMPO_COLS; c++)
                danger[cy][c] = true;
            break;

        case PONTO:
            danger[cy][cx] = true;
            break;

        case AREA:
            // Retângulo r×r com origem em (cx, cy)
            for (int row = cy; row < cy + r && row < CAMPO_ROWS; row++)
                for (int col = cx; col < cx + r && col < CAMPO_COLS; col++)
                    danger[row][col] = true;
            break;

        case ESFERA:
            // Círculo de raio r centrado em (cx, cy)
            for (int row = 0; row < CAMPO_ROWS; row++)
                for (int col = 0; col < CAMPO_COLS; col++) {
                    double dx = col - cx, dy = row - cy;
                    if (dx * dx + dy * dy <= (double)r * r)
                        danger[row][col] = true;
                }
            break;

        case DISPERSAO:
            // Pontos aleatórios espalhados (seed determinística por ataque)
            srand((unsigned)(atk->x * 31337u + atk->y * 9973u));
            for (int n = 0; n < r * 4; n++) {
                int row = rand() % CAMPO_ROWS;
                int col = rand() % CAMPO_COLS;
                danger[row][col] = true;
            }
            break;
    }
}

/* Janela central: campo de batalha com ♥ do player e zonas de perigo.
 * Se danger == NULL, exibe apenas o ♥ (fase de menu).
 */
static void draw_campo(CombateCtx *ctx,
                        const bool danger[CAMPO_ROWS][CAMPO_COLS])
{
    WINDOW *w = ctx->win_campo;
    werase(w);
    box(w, 0, 0);

    // Linha 1 interna: legenda do campo
    wattron(w, COLOR_PAIR(COR_NORMAL));
    mvwprintw(w, 1, 2, "[ CAMPO DE BATALHA ]");
    wattroff(w, COLOR_PAIR(COR_NORMAL));

    // Linhas 2..CAMPO_ROWS+1: células lógicas
    // Cada célula ocupa 2 colunas: col_screen = 2 + col_logico * 2
    for (int row = 0; row < CAMPO_ROWS; row++) {
        int screen_row = row + 2;
        if (screen_row >= CAM_WIN_H - 1) break;   // não ultrapassar borda

        for (int col = 0; col < CAMPO_COLS; col++) {
            int screen_col = 2 + col * 2;
            if (screen_col + 1 >= WIN_W - 1) break;

            bool is_player = (row == ctx->py && col == ctx->px);
            bool is_perigo = danger && danger[row][col];

            if (is_player) {
                // ♥  (coração vermelho em negrito)
                wattron(w, COLOR_PAIR(COR_HEART) | A_BOLD);
                mvwprintw(w, screen_row, screen_col, "\xe2\x99\xa5 "); // UTF-8: ♥
                wattroff(w, COLOR_PAIR(COR_HEART) | A_BOLD);
            } else if (is_perigo) {
                // ██ (bloco de perigo, fundo vermelho)
                wattron(w, COLOR_PAIR(COR_PERIGO));
                mvwprintw(w, screen_row, screen_col,
                          "\xe2\x96\x88\xe2\x96\x88"); // UTF-8: ██
                wattroff(w, COLOR_PAIR(COR_PERIGO));
            }
            // células vazias: não imprime nada (background limpo)
        }
    }

    wrefresh(w);
}

/* Janela inferior: grade 2×2 de ações.
 *
 *   +----ATACAR-----+  +---DESISTIR----+
 *   +-----ITEM------+  +----MERCY------+
 *
 * A opção selecionada é realçada em amarelo (COR_SEL).
 */
static void draw_menu(CombateCtx *ctx)
{
    WINDOW *w = ctx->win_menu;
    werase(w);
    box(w, 0, 0);

    // Calcula largura de cada célula do grid
    int btn_w   = (WIN_W - 6) / 2;   // espaço disponível para 2 botões + margens
    int col_pos[2] = { 2, 2 + btn_w + 2 };

    // Layout do grid: [acao][0]=linha_do_grid, [acao][1]=coluna_do_grid
    static const int GRID[ACAO_COUNT][2] = {
        {0, 0},   // ATACAR
        {0, 1},   // DESISTIR
        {1, 0},   // ITEM
        {1, 1},   // MERCY
    };

    for (int i = 0; i < ACAO_COUNT; i++) {
        int grid_row = GRID[i][0];
        int grid_col = GRID[i][1];
        int y   = 1 + grid_row * 3;     // cada botão ocupa 3 linhas
        int x   = col_pos[grid_col];
        bool sel = (ctx->acao_selecionada == (AcaoCombate)i);

        // Linha superior
        char border_h[128];
        memset(border_h, '-', (size_t)btn_w);
        border_h[btn_w] = '\0';

        if (sel) wattron(w, COLOR_PAIR(COR_SEL) | A_BOLD);
        else      wattron(w, COLOR_PAIR(COR_NORMAL));

        mvwprintw(w, y,     x, "+%s+", border_h);
        mvwprintw(w, y + 1, x, "|%-*s|", btn_w, MENU_LABEL[i]);
        mvwprintw(w, y + 2, x, "+%s+", border_h);

        if (sel) wattroff(w, COLOR_PAIR(COR_SEL) | A_BOLD);
        else      wattroff(w, COLOR_PAIR(COR_NORMAL));
    }

    wrefresh(w);
}

/* Linha de status fora das janelas: HP do player e nível. */
static void draw_status(CombateCtx *ctx)
{
    move(STATUS_ROW, WIN_X);
    clrtoeol();

    attron(COLOR_PAIR(COR_HP) | A_BOLD);
    printw(" HP: %d / %d", ctx->player->vida, ctx->player_vida_max);
    attroff(COLOR_PAIR(COR_HP) | A_BOLD);

    attron(COLOR_PAIR(COR_NORMAL));
    mvprintw(STATUS_ROW, WIN_X + WIN_W - 12, "LV: %-3d", ctx->player->level);
    attroff(COLOR_PAIR(COR_NORMAL));

    refresh();
}

// ═════════════════════════════════════════════════════════════════════
// Lógica de combate
// ═════════════════════════════════════════════════════════════════════

/* Fase de esquiva: player move ♥ durante ESQUIVA_FRAMES frames.
 * Usa nodelay para leitura não-bloqueante + usleep para controlar o FPS.
 * Ao final, verifica se o player está em uma célula de perigo.
 */
static void fase_esquiva(CombateCtx *ctx, AtaqueInimigo *atk)
{
    bool danger[CAMPO_ROWS][CAMPO_COLS];
    build_danger_map(danger, atk);

    // Aviso antes de começar
    snprintf(ctx->mensagem, sizeof(ctx->mensagem),
             "%s esta atacando! Desvie!", ctx->inimigo->nome);
    draw_inimigo(ctx);

    nodelay(stdscr, TRUE);   // getch() não bloqueia
   
    /*for (int f = 0; f < ESQUIVA_FRAMES; f=f+(f/2)) {
        
        draw_campo(ctx, danger);

        int ch = getch();
        switch (ch) {
            case KEY_LEFT:
                if (ctx->px > 0) ctx->px--;
                break;
            case KEY_RIGHT:
                if (ctx->px < CAMPO_COLS - 1) ctx->px++;
                break;
            case KEY_UP:
                if (ctx->py > 0) ctx->py--;
                break;
            case KEY_DOWN:
                if (ctx->py < CAMPO_ROWS - 1) ctx->py++;
                break;
            default:
                break;
        }

        usleep(ESQUIVA_FRAME_US);
    }*/

    nodelay(stdscr, FALSE);  // volta ao modo bloqueante

    // ── Verifica colisão ──────────────────────────────────────────────
    if (danger[ctx->py][ctx->px]) {
        ctx->player->vida -= atk->dano;
        snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                 "%s acertou! Voce sofreu %d de dano!",
                 ctx->inimigo->nome, atk->dano);
    } else {
        snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                 "Voce desviou do ataque de %s!", ctx->inimigo->nome);
    }
}

/* Escolhe um ataque aleatório do inimigo e inicia a fase de esquiva.
 * Se o inimigo não tiver ataques cadastrados, ele "passa a vez".
 */
static void turno_inimigo(CombateCtx *ctx)
{
    ArrayList *ataques = ctx->inimigo->ataques;

    if (!ataques || arraylist_size(ataques) <= 0) {
        snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                 "%s contempla voce em silencio.", ctx->inimigo->nome);
        return;
    }

    int idx = rand() % arraylist_size(ataques);
    AtaqueInimigo *atk = (AtaqueInimigo *)_arraylist_get(ataques, idx);

    fase_esquiva(ctx, atk);

    // Verifica derrota após o ataque
    if (ctx->player->vida <= 0) {
        ctx->player->vida = 0;
        ctx->estado = COMBATE_DERROTA;
        snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                 "Voce foi derrotado por %s...", ctx->inimigo->nome);
    }
}

/* Executa a ação escolhida pelo player.
 * Para ATACAR, ITEM e MERCY: ao fim, passa para o turno do inimigo
 * (a menos que o combate já tenha terminado).
 */
static void executar_acao(CombateCtx *ctx)
{
    switch (ctx->acao_selecionada)
    {
        // ── ATACAR ────────────────────────────────────────────────────
        case ACAO_ATACAR:
        {
            bool acertou = atacar_inimigo(ctx->player, ctx->inimigo);

            if (!acertou) {
                snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                         "O inimigo desviou! Voce errou o ataque.");
            } else {
                snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                         "Voce atacou %s!", ctx->inimigo->nome);
            }

            if (ctx->inimigo->vida <= 0) {
                ctx->inimigo->vida = 0;
                ctx->estado = COMBATE_VITORIA;
                snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                         "%s foi derrotado! Voce ganhou!", ctx->inimigo->nome);
                return;
            }
            break;
        }

        // ── DESISTIR ──────────────────────────────────────────────────
        case ACAO_DESISTIR:
            ctx->estado = COMBATE_FUGA;
            snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                     "Voce fugiu da batalha!");
            return;   // sem turno do inimigo

        // ── ITEM ──────────────────────────────────────────────────────
        case ACAO_ITEM:
            // TODO: abrir submenu de inventário e aplicar item
            snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                     "(Inventario: em construcao)");
            break;

        // ── MERCY ─────────────────────────────────────────────────────
        case ACAO_MERCY:
        {
            // Chance de sucesso cresce com o karma do player (max ~70%)
            int chance = 20 + ctx->player->karma * 5;
            if (chance > 70) chance = 70;

            if ((rand() % 100) < chance) {
                ctx->estado = COMBATE_MERCY_OK;
                snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                         "%s aceitou sua misericordia!",
                         ctx->inimigo->nome);
                ctx->player->karma++;
                return;
            }
            snprintf(ctx->mensagem, sizeof(ctx->mensagem),
                     "%s ignorou sua misericordia.", ctx->inimigo->nome);
            break;
        }

        default:
            break;
    }

    // ── Turno do inimigo (após ação não-terminal do player) ───────────
    if (ctx->estado == COMBATE_ATIVO)
        turno_inimigo(ctx);
}

// ═════════════════════════════════════════════════════════════════════
// API pública
// ═════════════════════════════════════════════════════════════════════

CombateCtx *combate_init(Player *player, Inimigo *inimigo)
{
    if (!player || !inimigo) return NULL;

    // Suporte a UTF-8 (♥ e ██)
    setlocale(LC_ALL, "");

    CombateCtx *ctx = malloc(sizeof(CombateCtx));
    if (!ctx) exit_with_error(Exception_AllocationError);

    ctx->player           = player;
    ctx->inimigo          = inimigo;
    ctx->player_vida_max  = player->vida;
    ctx->inimigo_vida_max = inimigo->vida > 0 ? inimigo->vida : 1;
    ctx->acao_selecionada = ACAO_ATACAR;
    ctx->estado           = COMBATE_ATIVO;
    ctx->mensagem[0]      = '\0';
    ctx->px               = CAMPO_COLS / 2;   // ♥ começa no centro
    ctx->py               = CAMPO_ROWS / 2;

    // Inicializa cores (requer initscr() já chamado)
    init_cores();

    // Cria as três janelas
    ctx->win_inimigo = newwin(INI_WIN_H, WIN_W, INI_WIN_Y, WIN_X);
    ctx->win_campo   = newwin(CAM_WIN_H, WIN_W, CAM_WIN_Y, WIN_X);
    ctx->win_menu    = newwin(MENU_WIN_H, WIN_W, MENU_WIN_Y, WIN_X);

    if (!ctx->win_inimigo || !ctx->win_campo || !ctx->win_menu)
        exit_with_error(Exception_AllocationError);

    // Verifica tamanho do terminal
    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);
    if (max_rows < MENU_WIN_Y + MENU_WIN_H || max_cols < WIN_X + WIN_W) {
        delwin(ctx->win_inimigo);
        delwin(ctx->win_campo);
        delwin(ctx->win_menu);
        free(ctx);
        endwin();
        fprintf(stderr,
                "[COMBATE] Terminal muito pequeno. "
                "Minimo recomendado: %d colunas x %d linhas.\n",
                WIN_X + WIN_W,
                MENU_WIN_Y + MENU_WIN_H);
        exit(1);
    }

    return ctx;
}

EstadoCombate combate_loop(CombateCtx *ctx)
{
    srand((unsigned)time(NULL));

    while (ctx->estado == COMBATE_ATIVO)
    {
        // ── Desenha tela completa ─────────────────────────────────────
        draw_inimigo(ctx);
        draw_campo(ctx, NULL);   // sem zona de perigo durante o menu
        draw_menu(ctx);
        draw_status(ctx);

        // ── Lê entrada do player ──────────────────────────────────────
        int ch = getch();

        switch (ch)
        {
            // Navegação no menu 2×2
            // Layout:  [ATACAR=0][DESISTIR=1]
            //          [ITEM=2  ][MERCY=3   ]
            case KEY_LEFT:
                // Move para a coluna esquerda (índice par)
                if (ctx->acao_selecionada & 1)
                    ctx->acao_selecionada =
                        (AcaoCombate)(ctx->acao_selecionada - 1);
                break;

            case KEY_RIGHT:
                // Move para a coluna direita (índice ímpar)
                if (!(ctx->acao_selecionada & 1))
                    ctx->acao_selecionada =
                        (AcaoCombate)(ctx->acao_selecionada + 1);
                break;

            case KEY_UP:
                // Sobe uma linha (ação - 2)
                if (ctx->acao_selecionada >= 2)
                    ctx->acao_selecionada =
                        (AcaoCombate)(ctx->acao_selecionada - 2);
                break;

            case KEY_DOWN:
                // Desce uma linha (ação + 2)
                if (ctx->acao_selecionada < 2)
                    ctx->acao_selecionada =
                        (AcaoCombate)(ctx->acao_selecionada + 2);
                break;

            case '\n':
            case KEY_ENTER:
            case ' ':
                executar_acao(ctx);
                break;

            default:
                break;
        }
    }

    // ── Tela final: exibe resultado e aguarda tecla ───────────────────
    draw_inimigo(ctx);
    draw_campo(ctx, NULL);
    draw_status(ctx);
    // Redesenha menu sem destaque para indicar fim de turno
    draw_menu(ctx);
    getch();

    return ctx->estado;
}

void combate_cleanup(CombateCtx *ctx)
{
    if (!ctx) return;
    if (ctx->win_inimigo) delwin(ctx->win_inimigo);
    if (ctx->win_campo)   delwin(ctx->win_campo);
    if (ctx->win_menu)    delwin(ctx->win_menu);
    free(ctx);
}
