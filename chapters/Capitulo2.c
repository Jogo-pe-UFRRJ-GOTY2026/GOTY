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

bool Capitulo2(Player* player)
{
    WINDOW *tela_capitulo2 = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    box(tela_capitulo2, 0, 0);
    wrefresh(tela_capitulo2);

    // -------------------------- Descrição do Cenário
    slow_mvwprintw(tela_capitulo2, "Você sobe as escadas, a composição delas passa de uma pedra suavemente queimada", 10, 6, 20);
    slow_mvwprintw(tela_capitulo2, "para algo semelhante a mármore branco e puro…", 11, 6, 20);
    slow_mvwprintw(tela_capitulo2, "A temperatura se torna levemente mais agradável como se no meio de um bosque,", 12, 6, 20);
    slow_mvwprintw(tela_capitulo2, "e aos poucos você consegue perceber vinhas soltas e flores no chão.", 13, 6, 20);
    slow_mvwprintw(tela_capitulo2, "Você se depara com uma porta massiva de madeira lindamente esculpida…", 14, 6, 20);

    napms(2500);

    werase(tela_capitulo2);
    box(tela_capitulo2, 0, 0);

    // Renderiza o sprite do Centauro (Aphanos)
    desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur_admiring_flowers.txt", 1, 5);

    slow_mvwprintw(tela_capitulo2, "Você abre a porta, e se depara com outra criatura mística, um centauro…", 28, 6, 20);
    slow_mvwprintw(tela_capitulo2, "Ele está parado com uma flor na mão enquanto olha para as flores no chão.", 29, 6, 20);

    napms(2000);

    // -------------------------- Diálogo Condicional
    if (player->medidor_lembranca[Aphanos] < 1)
    {
        const char *opcoes_dialogo[3] = {
            "1. Quem é você? Como um Centauro veio parar aqui?",
            "2. Eu vou mudar o meu destino, eu vou voltar a ser lembrado.",
            "3. Eu vou subir. Não importa o que esteja acima de mim."
        };

        int ja_realizou_pergunta[3] = {0};
        int opcao = 0;

        while (true)
        {
            werase(tela_capitulo2);
            box(tela_capitulo2, 0, 0);
            desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur.txt", 1, 5);

            // Menu de Opções
            for (int i = 0; i < 3; i++)
            {
                if (ja_realizou_pergunta[i])
                    wattron(tela_capitulo2, COLOR_PAIR(COR_OPCAO_INVALIDA));

                mvwprintw(tela_capitulo2, 31 + i, 6, opcoes_dialogo[i]);

                wattrset(tela_capitulo2, A_NORMAL);
            }

            wattron(tela_capitulo2, COLOR_PAIR(COR_DESTAQUE));
            mvwprintw(tela_capitulo2, getmaxy(tela_capitulo2) - 3, 4, "Pressione ENTER para iniciar o combate");
            wattroff(tela_capitulo2, COLOR_PAIR(COR_DESTAQUE));

            wrefresh(tela_capitulo2);

            int tecla = wgetch(tela_capitulo2);

            if (tecla == '1') opcao = 0;
            if (tecla == '2') opcao = 1;
            if (tecla == '3') opcao = 2;

            if (tecla == KEY_ENTER || tecla == '\n' || tecla == 10)
            {
                opcao = 2; // Força a opção de combate/passagem direta
            }

            if (!ja_realizou_pergunta[opcao])
            {
                ja_realizou_pergunta[opcao] = 1;

                werase(tela_capitulo2);
                box(tela_capitulo2, 0, 0);
                desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur.txt", 1, 5);

                mvwprintw(tela_capitulo2, 29, 6, "[Aphanos]");

                if (opcao == 0)
                {
                    slow_mvwprintw(tela_capitulo2, "Meu nome é Aphanos, eu fui apagado pelas Moiras e condenado ao Intervalo…", 30, 6, 20);
                }
                else if (opcao == 1)
                {
                    slow_mvwprintw(tela_capitulo2, "Ser lembrado? Eu lutei por isso! Eu perdi tudo por isso!", 30, 6, 20);
                    slow_mvwprintw(tela_capitulo2, "E o mundo ainda me arrancou do tempo como se eu nunca tivesse existido!", 31, 6, 20);
                }
                else if (opcao == 2)
                {
                    slow_mvwprintw(tela_capitulo2, "Então vem! Sobe se quiser! Mas não pense que destino vai te deixar passar ileso só porque você quer!", 30, 6, 20);
                    wrefresh(tela_capitulo2);
                    napms(1500);
                    break; // Sai do loop para iniciar a luta
                }

                wrefresh(tela_capitulo2);
                napms(2500);
            }
        }
    }
    else
    {
        // Caso o Player já tenha enfrentado ele antes
        werase(tela_capitulo2);
        box(tela_capitulo2, 0, 0);
        desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur.txt", 1, 5);

        mvwprintw(tela_capitulo2, 28, 6, "[Aphanos]");
        slow_mvwprintw(tela_capitulo2, "Você repete o mesmo erro novamente…", 29, 6, 20);
        slow_mvwprintw(tela_capitulo2, "Por algum acaso você realmente acha que suas escolhas fazem diferença?", 30, 6, 20);

        wrefresh(tela_capitulo2);
        napms(2500);
    }

    // -------------------------- Configuração do Combate
    AtaqueInimigo ataques[] = {
        criar_ataque(LINHA, 6, "}--)>", 1, HORIZONTAL, ESQUERDA_DIREITA, 2, 25),
        criar_ataque(LINHA, 6, "<(--{", 1, HORIZONTAL, DIREITA_ESQUERDA, 2, 25),
        criar_ataque(PAREDE, 5, ">", 1, VERTICAL, ESQUERDA_DIREITA, 8, 120),
        criar_ataque_area(5, "➴", 1, 0, 0, 80, 50),
        criar_ataque(BULLET, 8,"🍃",1, VERTICAL, CIMA_BAIXO, 3, 30)
    };

    Sprite_size size;
    size.x = 34; 
    size.y = 20;

    Inimigo *aphanos = criar_inimigo(160, "Aphanos", "assets/sprites/bosses/centaur.txt", size, 35, 23, Aphanos);

    // Falas de Re-encontro e Mecânicas de Luta
    aphanos->dialogo_reever_player = "Você repete o mesmo erro novamente… Por algum acaso você realmente acha que suas escolhas fazem diferença?";

    // Diálogos de Ataque recebido
    aphanos->dialogos_ataque[0] = "Finalmente um oponente digno… Nossa batalha será lendária!";
    aphanos->dialogos_ataque[1] = "Esse ataque fraco me fez lembrar da minha batalha contra Geras";
    aphanos->dialogos_ataque[2] = "Ataque formidável… Digno de um deus";
    aphanos->dialogos_ataque[3] = "Para quem já lutou contra Dionísio, isso parece brincadeira de criança.";
    aphanos->dialogos_ataque[4] = "Você realmente mostra força… Continue!";

    // Diálogos de Mercy (Piedade)
    aphanos->dialogos_mercy[0] = "Eu implorei por isso uma vez… de joelhos… e não recebi nada!";
    aphanos->dialogos_mercy[1] = "Misericórdia não mudará o seu destino!";
    aphanos->dialogos_mercy[2] = "Se vai lutar comigo, lute de verdade!";

    aphanos->numero_ataques = len(ataques);
    for (int i = 0; i < aphanos->numero_ataques; i++)
        aphanos->ataques[i] = ataques[i];

    werase(tela_capitulo2);
    wrefresh(tela_capitulo2);

    // Execução da luta
    EstadoRodada resultado_combate = iniciar_combate(player, aphanos);
    player->medidor_lembranca[Aphanos] += 1;

    // -------------------------- Pós-Combate
    werase(tela_capitulo2);
    box(tela_capitulo2, 0, 0);
    wrefresh(tela_capitulo2);

    if (resultado_combate == VITORIA && aphanos->vida <= 0)
    {
        // Roteiro: Derrotado por Ataque
        desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur_defeated.txt", 1, 5);
        mvwprintw(tela_capitulo2, 24, 6, "[Aphanos]");

        slow_mvwprintw(tela_capitulo2, "Vejo que…", 25, 6, 35);
        napms(1500);
        slow_mvwprintw(tela_capitulo2, "talvez você realmente tenha um destino à sua frente…", 26, 6, 35);
        napms(1500);
        slow_mvwprintw(tela_capitulo2, "um daqueles que eu nunca consegui alcançar…", 27, 6, 35);
        napms(1500);
        slow_mvwprintw(tela_capitulo2, "que pena… não poder…", 28, 6, 35);
        napms(1500);
        slow_mvwprintw(tela_capitulo2, "presenciá-lo", 29, 6, 45);

        player->NumeroAndar = Andar3;
        player->vida = vida_max_total(player);
    }
    else if (resultado_combate == VITORIA && aphanos->vida >= 0)
    {
        // Roteiro: Derrotado por Mercy
        desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur_defeated.txt", 1, 5);
        mvwprintw(tela_capitulo2, 24, 6, "[Aphanos]");

        slow_mvwprintw(tela_capitulo2, "Você tem um destino grandioso pela frente, vá, você merece ser lembrado!", 25, 6, 20);

        player->karma += 1;
        player->NumeroAndar = Andar3;
        player->vida = vida_max_total(player);
    }
    else
    {
        // Derrota do Player
        desenhar_sprite(tela_capitulo2, "assets/sprites/bosses/centaur.txt", 1, 5);
        mvwprintw(tela_capitulo2, 24, 6, "[Aphanos]");
        
        slow_mvwprintw(tela_capitulo2, "O seu destino já estava traçado desde o início…", 25, 6, 20);

        player->vida = vida_max_total(player);
        salvar_jogo(player);
        mostrar_tela_morte(player);
    }

    wrefresh(tela_capitulo2);
    napms(2500);
    apagar_janela(tela_capitulo2);

    return true;
}