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



bool Capitulo1(Player* player)
{
    WINDOW *tela_capitulo1 = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    box(tela_capitulo1, 0, 0);
    wrefresh(tela_capitulo1);


    // -------------------------- Introdução
    slow_mvwprintw(tela_capitulo1, "Você sobe as escadas e chega ao próximo andar.", 10, 6, 20);
    slow_mvwprintw(tela_capitulo1, "O chão do salão confere um calor desconfortável e uma névoa espessa cobre o ambiente.", 11, 6, 20);
    slow_mvwprintw(tela_capitulo1, "A névoa deixa o andar escuro e difícil de enxergar.", 12, 6, 20);
    slow_mvwprintw(tela_capitulo1, "Ao centro do salão, uma figura estranha dança alegremente.", 13, 6, 20);

    napms(2000);

    werase(tela_capitulo1);
    box(tela_capitulo1, 0, 0);

    desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_dancing.txt", 1, 5);

    slow_mvwprintw(tela_capitulo1, "Longos chifres crescem sobre sua cabeça e seus olhos brilham em vermelho carmesim.", 28, 6, 20);
    slow_mvwprintw(tela_capitulo1, "Você se aproxima…", 29, 6, 30);

    napms(2000);

    if (player->medidor_lembranca[Cruciator] < 1)
    {
        werase(tela_capitulo1);
        box(tela_capitulo1, 0, 0);

        desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_dancing.txt", 1, 5);

        mvwprintw(tela_capitulo1, 28, 6, "[???]");

        slow_mvwprintw(tela_capitulo1, "HAHAHA! Mais um!", 29, 6, 20);
        napms(2000);

        slow_mvwprintw(tela_capitulo1, "Desejando chegar ao topo, certo?", 30, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "A parte boa dessa função é que meu desejo é sempre realizado.", 31, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "CRUCIATOR!", 32, 6, 15);

        const char *opcoes_dialogo[3] = {
            "1. E você é?",
            "2. Você não parece querer sair da torre…",
            "3. Me deixa passar!"};

        int ja_realizou_pergunta[3] = {0};
        int opcao = 0;

        while (true)
        {
            werase(tela_capitulo1);
            box(tela_capitulo1, 0, 0);

            desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_dancing.txt", 1, 5);

            for (int i = 0; i < 3; i++)
            {
                if (ja_realizou_pergunta[i])
                    wattron(tela_capitulo1, COLOR_PAIR(COR_OPCAO_INVALIDA));

                mvwprintw(tela_capitulo1, 30 + i, 6, opcoes_dialogo[i]);

                wattrset(tela_capitulo1, A_NORMAL);
            }

            wattron(tela_capitulo1, COLOR_PAIR(COR_DESTAQUE));
            mvwprintw(tela_capitulo1, getmaxy(tela_capitulo1) - 3, 4, "Pressione ENTER para iniciar o combate");
            wattroff(tela_capitulo1, COLOR_PAIR(COR_DESTAQUE));

            wrefresh(tela_capitulo1);

            int tecla = wgetch(tela_capitulo1);

            if (tecla == '1')
                opcao = 0;
            if (tecla == '2')
                opcao = 1;
            if (tecla == '3')
                opcao = 2;

            if (tecla == KEY_ENTER || tecla == '\n' || tecla == 10)
            {
                opcao = 2;
            }

            if (!ja_realizou_pergunta[opcao])
            {
                ja_realizou_pergunta[opcao] = 1;

                werase(tela_capitulo1);
                box(tela_capitulo1, 0, 0);

                desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_dancing.txt", 1, 5);

                mvwprintw(tela_capitulo1, 28, 6, "[Cruciator]");

                if (opcao == 0)
                {
                    slow_mvwprintw(tela_capitulo1, "Um nobre demônio, meu caro.", 29, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Pode me chamar de Cruciator.", 30, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Estou aqui esperando a presença de viajantes como você.", 31, 6, 20);
                }
                else if (opcao == 1)
                {
                    slow_mvwprintw(tela_capitulo1, "Porque eu sairia?", 29, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Esse é um lugar esplendoroso!", 30, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Sabe quantas almas tentam passar por aqui?", 31, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "E uma fonte de diversão eterna!", 32, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Usque ad finem erit dierum.", 33, 6, 25);
                    napms(1000);

                }
                else if (opcao == 2)
                {
                    slow_mvwprintw(tela_capitulo1, "Não tão fácil assim, jovem!", 29, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "Vamos lá, divirta-me!", 30, 6, 20);
                    napms(2000);

                    slow_mvwprintw(tela_capitulo1, "(Ele aponta o tridente para você.)", 31, 6, 25);

                    break;
                }

                wrefresh(tela_capitulo1);
                napms(2000);
            }
        }
    }
    else
    {
        werase(tela_capitulo1);
        box(tela_capitulo1, 0, 0);

        desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_dancing.txt", 1, 5);

        mvwprintw(tela_capitulo1, 28, 6, "[Cruciator]");

        slow_mvwprintw(tela_capitulo1, "Vou acabar com isso rápido desta vez.", 29, 6, 20);

        napms(2000);
    }








    //  --- Combate ---
    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 5, "-------<⇶", 1, HORIZONTAL, ESQUERDA_DIREITA, 2, 30),
        criar_ataque(LINHA, 5, "⬱>-------", 1, HORIZONTAL, DIREITA_ESQUERDA, 2, 30),
        criar_ataque(PAREDE, 4, "V", 1, HORIZONTAL, CIMA_BAIXO, 10, 180), //o emoji de fogo quebra no linux
        criar_ataque_area(4, "◮", 1, 0, 0, 90, 60)
    };

    Sprite_size size;
    size.x = 19;
    size.y = 21;

    Inimigo *cruciator = criar_inimigo(5, "Cruciator","assets/sprites/bosses/demon.txt", size, 35, 23, Cruciator);

    cruciator->dialogo_reever_player = "Olha só, olha só…Acho que alguém gostou da diversão de antes! Hahaha";

    cruciator->dialogos_mercy[0] = "Misericórdia? HAHAHAHAHA! Muitas almas já imploraram por isso… de onde eu vim.";
    cruciator->dialogos_mercy[1] = "In quolibet proelio, misericordes pereunt, dum immisericordes supervivent propter ironiam misericordiae.";
    cruciator->dialogos_mercy[2] = "Não… não, não! SEM misericórdia aqui!";

    cruciator->dialogos_ataque[0] = "HAHAHA! Algo assim não é capaz de ferir nem uma mosca!";
    cruciator->dialogos_ataque[1] = "Você acha que conseguirá passar?";
    cruciator->dialogos_ataque[2] = "HAHAHAHA! Tenta de novo! Isso não arranha nem o meu orgulho!";
    cruciator->dialogos_ataque[3] = "Urgh… dor? Em MIM?!?!?";
    cruciator->dialogos_ataque[4] = "Não vou perder para um mero humano qualquer!";

    cruciator->numero_ataques = len(ataques);
    for (int i = 0; i < cruciator->numero_ataques; i++)
        cruciator->ataques[i] = ataques[i];

    werase(tela_capitulo1);
    wrefresh(tela_capitulo1);

    EstadoRodada resultado_combate = iniciar_combate(player, cruciator);
    player->medidor_lembranca[Cruciator] += 1;

    // --- Pos combate ---
    werase(tela_capitulo1);
    box(tela_capitulo1, 0, 0);
    wrefresh(tela_capitulo1);

    if (resultado_combate == VITORIA && cruciator->vida <= 0)
    {
        // Jogador matou o boss
        desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_defeated.txt", 1, 5);

        mvwprintw(tela_capitulo1, 22, 6, "[Cruciator]");

        slow_mvwprintw(tela_capitulo1, "Desgraçado… eu… depois de tanto tempo…", 23, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "Não… não pode ser…", 24, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "Eu não posso simplesmente…", 25, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "apagar assim…", 26, 6, 20);

        player->NumeroAndar = Andar2;
        player->vida = vida_max_total(player);
    }
    else if (resultado_combate == VITORIA && cruciator->vida >= 0)
    {
        desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon_defeated.txt", 1, 5);

        mvwprintw(tela_capitulo1, 22, 6, "[Cruciator]");

        slow_mvwprintw(tela_capitulo1, "Misericórdia…? HAHAHAHAHA!", 23, 6, 20);
        napms(2000);

        slow_mvwprintw(tela_capitulo1, "Que ironia deliciosa…", 24, 6, 20);
        napms(2000);

        slow_mvwprintw(tela_capitulo1, "Depois de tudo que vi e fiz…", 25, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "você ainda tenta me poupar?", 26, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "SAIA DAQUI!", 27, 6, 20);
        napms(1500);

        slow_mvwprintw(tela_capitulo1, "E não ouse voltar a me entreter de novo!", 28, 6, 20);
        napms(1500);

        player->karma += 1;
        player->NumeroAndar = Andar2;
    }
    else
    {
        desenhar_sprite(tela_capitulo1, "assets/sprites/bosses/demon.txt", 1, 5);

        mvwprintw(tela_capitulo1, 22, 6, "[Cruciator]");

        slow_mvwprintw(tela_capitulo1, "HAHAHAHA! Já acabou? Tão pouco… eu queria mais!", 23, 6, 20);

        player->vida = vida_max_total(player);

        salvar_jogo(player);

        mostrar_tela_morte(player);
    }
    wrefresh(tela_capitulo1);
    napms(2200);
    apagar_janela(tela_capitulo1);
}