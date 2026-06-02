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

bool Capitulo4(Player *player)
{
    WINDOW *tela_capitulo3 = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    box(tela_capitulo3, 0, 0);
    wrefresh(tela_capitulo3);

    // -------------------------- Descrição do Cenário
    slow_mvwprintw(tela_capitulo3, "Você abre a penúltima porta antes da chegada ao topo, um cheiro forte penetra suas narinas, e uma nuvem de cinzas levanta no ar.", 10, 6, 20);
    slow_mvwprintw(tela_capitulo3, "A parede desse andar está danificada, e queimando com chamas incessantes\nque parecem que não se extinguem a séculos e impossibilitam qualquer passagem ou atalho", 11, 6, 20);
    slow_mvwprintw(tela_capitulo3, "no meio do salão em meio a uma pilha de moedas repousa um dragão dormindo,", 13, 6, 20);
    slow_mvwprintw(tela_capitulo3, "", 14, 6, 20);

    napms(2500);

    werase(tela_capitulo3);
    box(tela_capitulo3, 0, 0);

    // Renderiza o sprite do Centauro (Aphanos)
    desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon_sleeping.txt", 1, 5);

    slow_mvwprintw(tela_capitulo3, "Conforme você dá seu próximo passo parece que o ar se torna mais tenso e pesado e o chão treme levemente.", 28, 6, 20);
    slow_mvwprintw(tela_capitulo3, "e ele desperta.", 29, 6, 20);

    napms(2000);

    // -------------------------- Diálogo Condicional
    if (player->medidor_lembranca[Iowa] < 1)
    {
        const char *opcoes_dialogo[3] = {
            "1. Um dragão?? O que te trouxe até o alto da torre?",
            "2. Você também deseja sair? Ser lembrado",
            "3. Eu ainda preciso subir."
        };

        int ja_realizou_pergunta[3] = {0};
        int opcao = 0;

        while (true)
        {
            werase(tela_capitulo3);
            box(tela_capitulo3, 0, 0);
            desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon.txt", 1, 5);

            // Menu de Opções
            for (int i = 0; i < 3; i++)
            {
                if (ja_realizou_pergunta[i])
                    wattron(tela_capitulo3, COLOR_PAIR(COR_OPCAO_INVALIDA));

                mvwprintw(tela_capitulo3, 31 + i, 6, opcoes_dialogo[i]);

                wattrset(tela_capitulo3, A_NORMAL);
            }

            wattron(tela_capitulo3, COLOR_PAIR(COR_DESTAQUE));
            mvwprintw(tela_capitulo3, getmaxy(tela_capitulo3) - 3, 4, "Pressione ENTER para iniciar o combate");
            wattroff(tela_capitulo3, COLOR_PAIR(COR_DESTAQUE));

            wrefresh(tela_capitulo3);

            int tecla = wgetch(tela_capitulo3);

            if (tecla == '1')
                opcao = 0;
            if (tecla == '2')
                opcao = 1;
            if (tecla == '3')
                opcao = 2;

            if (tecla == KEY_ENTER || tecla == '\n' || tecla == 10)
            {
                opcao = 2; // Força a opção de combate/passagem direta
            }

            if (!ja_realizou_pergunta[opcao])
            {
                ja_realizou_pergunta[opcao] = 1;

                werase(tela_capitulo3);
                box(tela_capitulo3, 0, 0);
                desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon.txt", 1, 5);

                mvwprintw(tela_capitulo3, 29, 6, "[Iowa]");

                if (opcao == 0)
                {
                    slow_mvwprintw(tela_capitulo3, "Sou Iowa, uma entidade primordial do Intervalo.", 30, 6, 20);
                    slow_mvwprintw(tela_capitulo3, "Em verdade, já não me recordo os caminhos que me trouxeram a este lugar.",31,6,20);
                    slow_mvwprintw(tela_capitulo3, "Contudo, recordo-me de ti,",32,6,20);
                    slow_mvwprintw(tela_capitulo3, player->nome, 32, 33, 20);
                    slow_mvwprintw(tela_capitulo3, ", assim como de inúmeras almas que vagaram por este espaço ao longo das eras.", 32, 33+strlen(player->nome), 20);
                }
                else if (opcao == 1)
                {
                    
                }
                else if (opcao == 2)
                {
                    
                    napms(1500);
                    break; // Sai do loop para iniciar a luta
                }

                wrefresh(tela_capitulo3);
                napms(2500);
            }
        }
    }
    else
    {
        // Caso o Player já tenha enfrentado ele antes
        werase(tela_capitulo3);
        box(tela_capitulo3, 0, 0);
        desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/centaur.txt", 1, 5);

        mvwprintw(tela_capitulo3, 28, 6, "[Iowa]");
        slow_mvwprintw(tela_capitulo3, "Você repete o mesmo erro novamente…", 29, 6, 20);
        slow_mvwprintw(tela_capitulo3, "Por algum acaso você realmente acha que suas escolhas fazem diferença?", 30, 6, 20);

        wrefresh(tela_capitulo3);
        napms(2500);
    }

    // -------------------------- Configuração do Combate
    AtaqueInimigo ataques[] = {
        criar_ataque(BULLET, 8, "🔥", 1, VERTICAL, CIMA_BAIXO, 2, 25),
        criar_ataque(BULLET, 8, "🔥", 1, VERTICAL, BAIXO_CIMA, 2, 25),
        criar_ataque(LASER, 12, "🔥", 0, VERTICAL, NAO_IMPORTA, 80, 120),
        criar_ataque(PAREDE,8,"†",1, HORIZONTAL, BAIXO_CIMA, 8, 110),
        criar_ataque_area(5, "🔥", 1, 0, 0, 80, 50),
        criar_ataque_area(7, "🔥", 1, 4, 4, 90, 90),
        criar_ataque_area(7, "🔥", 1, 4, 4, 90, 90)
    };

    Sprite_size size;
    size.x = 34;
    size.y = 20;

    Inimigo *iowa = criar_inimigo(185, "Iowa", "assets/sprites/bosses/dragon.txt", size, 35, 23, Iowa);

    // Falas de Re-encontro e Mecânicas de Luta
    iowa->dialogo_reever_player = "Vejo que retornaste… curioso. Poucos escolhem desafiar o fogo uma segunda vez.";

    // Diálogos de Ataque recebido
    iowa->dialogos_ataque[0] = "Teu golpe carrega força… mas ainda não suficiente.";
    iowa->dialogos_ataque[1] = "Mostra-me mais.";
    iowa->dialogos_ataque[2] = "Mesmo as chamas respeitam aqueles que persistem.";
    iowa->dialogos_ataque[3] = "…Interessantissímo";
    iowa->dialogos_ataque[4] = "Você carrega determinação, não permita que ela se extingua… Prossiga!";

    // Diálogos de Mercy (Piedade)
    iowa->dialogos_mercy[0] = "Recusas o combate… diante de mim?";
    iowa->dialogos_mercy[1] = "Curioso… há força também naquele que escolhe não ferir.";
    iowa->dialogos_mercy[2] = "Talvez tua verdadeira determinação não esteja em tua espada.";

    iowa->numero_ataques = len(ataques);
    for (int i = 0; i < iowa->numero_ataques; i++)
        iowa->ataques[i] = ataques[i];

    werase(tela_capitulo3);
    wrefresh(tela_capitulo3);

    // Execução da luta
    EstadoRodada resultado_combate = iniciar_combate(player, iowa);
    player->medidor_lembranca[Iowa] += 1;

    // -------------------------- Pós-Combate
    werase(tela_capitulo3);
    box(tela_capitulo3, 0, 0);
    wrefresh(tela_capitulo3);

    if (resultado_combate == VITORIA && iowa->vida <= 0)
    {
        // Roteiro: Derrotado por Ataque
        desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon_defeated.txt", 1, 5);
        mvwprintw(tela_capitulo3, 24, 6, "[Iowa]");

        slow_mvwprintw(tela_capitulo3, "Então… até mesmo minhas chamas chegaram ao fim…", 25, 6, 35);
        napms(1500);
        slow_mvwprintw(tela_capitulo3, "Vá… sobe enquanto ainda és lembrado.", 26, 6, 35);
        napms(1500);

        player->NumeroAndar = Andar3;
        player->vida = vida_max_total(player);
    }
    else if (resultado_combate == VITORIA && iowa->vida >= 0)
    {
        // Roteiro: Derrotado por Mercy
        desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon_defeated.txt", 1, 5);
        mvwprintw(tela_capitulo3, 24, 6, "[Iowa]");

        slow_mvwprintw(tela_capitulo3, "Tu venceste… e ainda assim escolheste poupar-me.", 25, 6, 20);
        slow_mvwprintw(tela_capitulo3, "Passe. O caminho ao topo está aberto… minhas chamas não mais te impedirão.", 26, 6, 20);

        player->karma += 1;
        player->NumeroAndar = Andar3;
        player->vida = vida_max_total(player);
    }
    else
    {
        // Derrota do Player
        desenhar_sprite(tela_capitulo3, "assets/sprites/bosses/dragon.txt", 1, 5);
        mvwprintw(tela_capitulo3, 24, 6, "[Iowa]");

        slow_mvwprintw(tela_capitulo3, "Se seu espírito deseja completar a Torre, então se prepare, e tente novamente! Você só passará quando merecer.", 25, 6, 20);

        player->vida = vida_max_total(player);
        salvar_jogo(player);
        mostrar_tela_morte(player);
    }

    wrefresh(tela_capitulo3);
    napms(2500);
    apagar_janela(tela_capitulo3);

    return true;
}