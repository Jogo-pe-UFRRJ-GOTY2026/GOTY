#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "CAPITULO.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"
#include "../system/Combate.h"
#include "../utils/utils.h"
#include "../system/Save.h"


void Prologo_pt2(Player *player)
{
    WINDOW *tela_prologopt2 = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    box(tela_prologopt2, 0, 0);
    wrefresh(tela_prologopt2);

    slow_mvwprintw(tela_prologopt2,"Você entra no térreo da torre, o ar gélido do lugar paira perante sua face,",10, 6, 20);
    slow_mvwprintw(tela_prologopt2,"aquecendo-se conforme flui para o exterior. O térreo comporta um breve jardim repleto de gérberas,",11, 6, 20);
    slow_mvwprintw(tela_prologopt2,"tingindo a paisagem em cores brancas e vermelhas.",12, 6, 20);
    napms(2000);

    werase(tela_prologopt2);
    box(tela_prologopt2, 0, 0);
    desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt",1,5);
    wrefresh(tela_prologopt2);

    slow_mvwprintw(tela_prologopt2,"Nas escadas levando ao primeiro andar, sentado há um cavaleiro. Ao seu lado direito, um longo machado",32, 6, 20);
    slow_mvwprintw(tela_prologopt2,"e a seu lado esquerdo, um escudo.",33, 6, 20);
    napms(2000);

    werase(tela_prologopt2);
    box(tela_prologopt2, 0, 0);
    desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt", 1, 5);
    wrefresh(tela_prologopt2);

    slow_mvwprintw(tela_prologopt2, "Silêncio absoluto.", 32, 6, 30);
    napms(2000);

    // --- Dialogo condicional ---
    if (player->medidor_lembranca[Hollow_Knight] < 1)
    {
        werase(tela_prologopt2);
        box(tela_prologopt2, 0, 0);
        desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt", 1, 5);
        mvwprintw(tela_prologopt2, 32, 6, "[Cavaleiro]");
        slow_mvwprintw(tela_prologopt2, "...mais um.", 33, 6, 30);
        napms(2000);
        slow_mvwprintw(tela_prologopt2,
                       "Você ainda tem esperança nos olhos, deve ser uma das suas primeiras vezes, não é?",
                       34, 6, 20);
        napms(1200);

        // --- Menu de dialogo ---
        const char *opcoes_dialogo[3] = {
            "1. Quem é você? O que faz aqui?",
            "2. Você tentou subir a torre tambem?",
            "3. Eu preciso passar."};

        int ja_realizou_pergunta[3] = {0};
        int opcao = 0;

        while (true)
        {
            werase(tela_prologopt2);
            box(tela_prologopt2, 0, 0);
            desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt", 1, 5);
            for (int i = 0; i < 3; i++)
            {
                if (ja_realizou_pergunta[i])
                    wattron(tela_prologopt2, COLOR_PAIR(COR_OPCAO_INVALIDA));

                mvwprintw(tela_prologopt2, 32 + i, 6, opcoes_dialogo[i]);

                wattrset(tela_prologopt2, A_NORMAL);
            }

            wattron(tela_prologopt2, COLOR_PAIR(COR_DESTAQUE));
            mvwprintw(tela_prologopt2, getmaxy(tela_prologopt2) - 3, 4,
                      "Pressione ENTER para iniciar o combate");
            wattroff(tela_prologopt2, COLOR_PAIR(COR_DESTAQUE));
            wrefresh(tela_prologopt2);

            int tecla = wgetch(tela_prologopt2);
            if (tecla == '1')
                opcao = 0;
            if (tecla == '2')
                opcao = 1;
            if (tecla == '3')
                opcao = 2;

            if (tecla == KEY_ENTER || tecla == '\n' || tecla == 10)
            {
                werase(tela_prologopt2);
                box(tela_prologopt2, 0, 0);
                desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt", 1, 5);
                mvwprintw(tela_prologopt2, 32, 6, "[Cavaleiro]");
                wrefresh(tela_prologopt2);
                // "Eu preciso passar."
                slow_mvwprintw(tela_prologopt2, "O Cavaleiro lentamente pega seu machado e escudo.", 33, 6, 20);
                napms(2000);

                werase(tela_prologopt2);
                box(tela_prologopt2, 0, 0);
                desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow.txt", 1, 5);

                mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
                wrefresh(tela_prologopt2);

                slow_mvwprintw(tela_prologopt2, "Não posso deixar que você suba…", 23, 6, 25);
                napms(2000);

                wattron(tela_prologopt2, A_BOLD);
                slow_mvwprintw(tela_prologopt2, "Você NÃO É MELHOR do que EU!", 24, 6, 20);
                wattrset(tela_prologopt2, A_NORMAL);
                napms(2000);
                break;
            }
            if (!ja_realizou_pergunta[opcao])
            {
                ja_realizou_pergunta[opcao] = 1;

                werase(tela_prologopt2);
                box(tela_prologopt2, 0, 0);
                desenhar_sprite(tela_prologopt2, "assets/sprites/buildings/terreo.txt", 1, 5);
                mvwprintw(tela_prologopt2, 32, 6, "[Cavaleiro]");
                wrefresh(tela_prologopt2);

                if (opcao == 0)
                {
                    // "Quem e voce? O que faz aqui?"
                    slow_mvwprintw(tela_prologopt2, "...", 33, 6, 60);
                    napms(2000);
                    slow_mvwprintw(tela_prologopt2,
                                   "Eu já tive algo que você poderia chamar de 'nome'.",
                                   34, 6, 20);
                    napms(2000);
                    slow_mvwprintw(tela_prologopt2,
                                   "Não me lembro mais de quem eu era. A torre me mantém vivo.",
                                   35, 6, 20);
                }
                else if (opcao == 1)
                {
                    // "Voce tentou subir a torre tambem?"
                    slow_mvwprintw(tela_prologopt2, "Tentei.", 33, 6, 30);
                    napms(2000);
                    slow_mvwprintw(tela_prologopt2, "Incontáveis vezes.", 34, 6, 30);
                    napms(2000);
                    slow_mvwprintw(tela_prologopt2, "E em todas eu fui derrotado.", 35, 6, 25);
                }
                else if (opcao == 2)
                {
                    // "Eu preciso passar."
                    slow_mvwprintw(tela_prologopt2,
                                   "O Cavaleiro lentamente pega seu machado e escudo.",
                                   33, 6, 20);
                    napms(2000);

                    werase(tela_prologopt2);
                    box(tela_prologopt2, 0, 0);
                    desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow.txt", 1, 5);

                    mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
                    wrefresh(tela_prologopt2);

                    slow_mvwprintw(tela_prologopt2, "Não posso deixar que você suba…", 23, 6, 25);
                    napms(2000);

                    wattron(tela_prologopt2, A_BOLD);
                    slow_mvwprintw(tela_prologopt2,"Você NÃO É MELHOR do que EU!",24, 6, 20);
                    wattrset(tela_prologopt2, A_NORMAL);
                    napms(2000);

                    break;
                }

                wrefresh(tela_prologopt2);
                napms(2000);;
            }
        }
    }
    else
    {
        // Segunda vez em diante
        werase(tela_prologopt2);
        box(tela_prologopt2, 0, 0);
        desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow.txt", 1, 5);
        mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
        slow_mvwprintw(tela_prologopt2,"Você é estranhamente insistente, estranho…",23, 6, 25);
        wrefresh(tela_prologopt2);
        napms(2000);
    }

    // --- Combate ---
    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 5, "█▙▔▔▔", 1, HORIZONTAL, DIREITA_ESQUERDA, 2, 30),
        criar_ataque(BULLET, 2, "†", 1, VERTICAL, CIMA_BAIXO, 4, 30),
        criar_ataque(BULLET, 3, "✱", 1, HORIZONTAL, ESQUERDA_DIREITA, 5, 30)};

    Sprite_size size;
    size.x = 33;
    size.y = 20;

    Inimigo *hollow_knight = criar_inimigo(100, "Hollow Knight",
                                           "assets/sprites/bosses/hollow.txt", size, 40, 20, Hollow_Knight);

    hollow_knight->dialogo_reever_player = "Você é estranhamente insistente, estranho…";

    hollow_knight->dialogos_mercy[0] = "Por que está hesitando?";
    hollow_knight->dialogos_mercy[1] = "Misericórdia não vai te levar longe nesta torre...";
    hollow_knight->dialogos_mercy[2] = "Não me olhe desse jeito... eu já fui como você.";

    hollow_knight->dialogos_ataque[0] = "Não fez nem cócegas...";
    hollow_knight->dialogos_ataque[1] = "Acha mesmo que pode me vencer?";
    hollow_knight->dialogos_ataque[2] = "Eu já enfrentei coisas muito piores que você.";
    hollow_knight->dialogos_ataque[3] = "Impressionante...";
    hollow_knight->dialogos_ataque[4] = "Não... você não pode chegar mais longe do que eu.";

    hollow_knight->numero_ataques = len(ataques);
    for (int i = 0; i < hollow_knight->numero_ataques; i++)
        hollow_knight->ataques[i] = ataques[i];

    werase(tela_prologopt2);
    wrefresh(tela_prologopt2);

    EstadoRodada resultado_combate = iniciar_combate(player, hollow_knight);
    player->medidor_lembranca[Hollow_Knight] += 1;

    // --- Pos combate ---
    werase(tela_prologopt2);
    box(tela_prologopt2, 0, 0);
    wrefresh(tela_prologopt2);

    if (resultado_combate == VITORIA && hollow_knight->vida <= 0)
    {
        // Jogador matou o boss
        desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow_defeated.txt", 1, 5);
        mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
        slow_mvwprintw(tela_prologopt2, "Eu falhei diante da Torre.", 23, 6, 25);
        slow_mvwprintw(tela_prologopt2, "Leve meu eco com você, e não cometa os mesmos erros.", 24, 6, 25);
        napms(2000);
        werase(tela_prologopt2);
        box(tela_prologopt2,0,0);
        wrefresh(tela_prologopt2);
        napms(2000);
        slow_mvwprintw(tela_prologopt2,"... talvez você chegue mais longe do que eu.",24, 6, 25);
        player->NumeroAndar=Andar1;
        // salvar_jogo(player);
    }
    else if (resultado_combate == VITORIA && hollow_knight->vida >= 0)
    {
        // Jogador deu misericordia
        desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow_defeated.txt", 1, 5);
        mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
        slow_mvwprintw(tela_prologopt2, "(ofegante) ...você venceu.", 23, 6, 20);
        napms(2000);
        slow_mvwprintw(tela_prologopt2, "Talvez você realmente seja diferente dos outros.", 24, 6, 20);
        napms(2000);
        slow_mvwprintw(tela_prologopt2,"Vá em frente, viajante... não deixe a torre apagar quem você é.",25, 6, 20);
        player->karma += 1;
        player->NumeroAndar=Andar1;
        // salvar_jogo(player);
    }
    else
    {
        desenhar_sprite(tela_prologopt2, "assets/sprites/bosses/hollow.txt", 1, 5);
        mvwprintw(tela_prologopt2, 22, 6, "[Cavaleiro]");
        slow_mvwprintw(tela_prologopt2, "Eu avisei... você ainda não é forte o bastante.", 23, 6, 20);
        player->vida = vida_max_total(player);
        salvar_jogo(player);

        mostrar_tela_morte(player);
    }
    wrefresh(tela_prologopt2);
    napms(2200);
    apagar_janela(tela_prologopt2);
}