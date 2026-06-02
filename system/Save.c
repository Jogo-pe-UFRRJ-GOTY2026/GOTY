#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../objects/Player.h"
#include "../objects/Inventario.h"
#include "../utils/utils.h"
#include "../chapters/CAPITULO.h"

#define NUM_OF_ELEMENTS 1
Player *carregar_salvamento()
{

    FILE *save_file = fopen("saves/player_save.bin", "rb");
    if (save_file == NULL)
    {
        // perror("Test Error: Erro ao ler arquivo de salvamento");
        return NULL;
    }

    Player *player = malloc(sizeof(Player));
    if (player == NULL)
    {
        // perror("Test Error: Erro ao alocar estrutura player");
        fclose(save_file);
        return NULL;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if (fread(player, sizeof(Player), NUM_OF_ELEMENTS, save_file) != NUM_OF_ELEMENTS)
    {
        // perror("Test Error: Erro ao ler estrutura player do arquivo de salvamento");
        free(player);
        fclose(save_file);
        return NULL;
    }
    fclose(save_file);

    return player;
}

bool salvar_jogo(Player *player)
{
    FILE *save_file = fopen("saves/player_save.bin", "wb");
    if (save_file == NULL)
    {
        perror("Test Error: Erro ao criar arquivo de salvamento");
        return false;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if (fwrite(player, sizeof(Player), NUM_OF_ELEMENTS, save_file) != NUM_OF_ELEMENTS)
    {
        perror("Test Error: Erro ao escrever estrutura player no arquivo de salvamento");
        fclose(save_file);
        return false;
    }
    fclose(save_file);
    return true;
}

void ponto_save(Player *player)
{
    WINDOW *ponto_save_window = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    keypad(ponto_save_window, TRUE);
    nodelay(ponto_save_window, TRUE);
    bool descansando = true;
    int tecla;

    while (descansando)
    {
        werase(ponto_save_window);
        box(ponto_save_window, 0, 0);
        desenhar_sprite(ponto_save_window, "assets/sprites/buildings/save_point.txt", 5, 5);

        tecla = wgetch(ponto_save_window);

        switch (tecla)
        {
        case '1':
            salvar_jogo(player);
            wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
            mvwprintw(ponto_save_window, getmaxy(ponto_save_window) - 5, 5, "Progresso salvo com sucesso");
            wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
            wrefresh(ponto_save_window);
            napms(1000);

            break;

        case '2':
            salvar_jogo(player);
            wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
            mvwprintw(ponto_save_window, getmaxy(ponto_save_window) - 3, 5, "Progresso salvo com sucesso. Fechando jogo…");
            wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE) | A_BOLD);
            wrefresh(ponto_save_window);
            napms(1500);
            apagar_janela(ponto_save_window);
            endwin();
            free(player);
            exit(EXIT_SUCCESS);

        case '3':
            werase(ponto_save_window);
            desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
            char *opcoes_dialogo[8];
            char *respostas[8];
            int num_perguntas; // max 3, min 0
            int ja_realizou_pergunta[3] = {0};
            int opcao;

            switch (player->NumeroAndar)
            {
            case Andar1: // depois do hollow knight
                num_perguntas = 6;

                opcoes_dialogo[0] = "1. O que é este lugar?";
                opcoes_dialogo[1] = "2. Quem é você?";
                opcoes_dialogo[2] = "3. O que é a Torre dos Ecos?";
                opcoes_dialogo[3] = "4. Quem era aquele guerreiro?";
                opcoes_dialogo[4] = "5. Aquele cavaleiro… ele parecia tão… indignado";

                respostas[0] = "Chamamos este lugar de Intervalo. Nem vida, nem morte. Apenas aquilo que restou depois que ambos passaram.";
                respostas[1] = "Sou apenas o Vigia. Observo os que chegam… e os que desaparecem.";
                respostas[2] = "A Torre não foi construída. Ela nasceu. Ela não é do inferno, nem do paraíso, nem do purgatório…";
                respostas[3] = "Mais uma alma que tentou vencer a Torre. O que restou dele ainda empunha a espada.";
                respostas[4] = "Ele tentou centenas de vezes chegar ao topo, sem sucesso. Acredito que ele não quer que sofram o mesmo destino dele, e por isso guarda a entrada";

                break;

            case Andar2: // depois do cruciator
                num_perguntas = 4;

                opcoes_dialogo[0] = "1. Existe algo ou alguém que nunca será esquecido?";
                opcoes_dialogo[1] = "2. O que são Ecos?";
                opcoes_dialogo[2] = "3. Por que sinto como se alguem estivesse me observando?";
                opcoes_dialogo[3] = "4. Quem era Cruciator?";

                respostas[0] = "Dentre os que não se esqueceram de tudo ainda, falam acerca do strogonoff do IM da UFRRJ.";
                respostas[1] = "Ecos são memórias que ecoam por todo o Intervalo, fazendo com que você seja lembrado por um longo tempo por aqui, mas não 'Ad Eternum'.";
                respostas[2] = "Porque está. Há olhos no Intervalo que permanecem abertos mesmo quando tudo ao redor adormece.";
                respostas[3] = "Seu nome completo é Cruciator ad Eternum. Ele foi considerado o Funcionário do Século por Lucifer e \n recebeu como prêmio a oportunidade de se 'entreter' com o sofrimento das almas do Intervalo que chegam até ele — e não o contrário.";
                break;
                
            case Andar3: // depois do Aphanos
                num_perguntas = 6;

                opcoes_dialogo[0] = "1. Por que eu continuo retornando? Porque não desapareço ao morrer?";
                opcoes_dialogo[1] = "2. Já houve alguém como eu aqui?";
                opcoes_dialogo[2] = "3. Por que Aphanos tinha uma visão tão melancólica do destino?";
                opcoes_dialogo[3] = "4. Por que Aphanos se envolveu com as Moiras?";
                opcoes_dialogo[4] = "5. O quanto você acredita no destino, que as cosias estão pré-determinadas? ";
                opcoes_dialogo[5] = "6. Por que o nome dele é Aphanos?";

                respostas[0] = "Porque seu medidor de memórias ainda não esvaziou… Você não tem mais pra onde ir, então volta pra cá.";
                respostas[1] = "Mais do que você imagina. Menos do que a Torre gostaria.";
                respostas[2] = "Aphanos fez um acordo com as Moiras, propositalmente impossível de ser realizado.";
                respostas[3] = "Sua esposa adquiriu uma doença terminal, então ele, sem ouvir todo o acordo, curou a esposa em troca de Aphanos assassinar o próprio pai.";
                respostas[4] = "Não o bastante, ou talvez não o necessário. Gosto de pensar que possuo livre-arbítrio.";
                respostas[5] = "Aphanos significa invisível, desde o nascimento ele era condenado a invisibilidade, a ver sua esposa e família esquecendo dele…";

                break;

            case Andar4: // depois do Cerberus, bixo de estimação do rei caido
                num_perguntas = 5;

                opcoes_dialogo[0] = "1. Por que sinto minhas memórias voltando?";
                opcoes_dialogo[1] = "2. Quem eu fui antes de chegar aqui?";
                opcoes_dialogo[2] = "3. Como um Cerberus foi parar aqui?";
                opcoes_dialogo[3] = "4. Os seres que estão morando na torre já viveram na Terra?"; 
                opcoes_dialogo[4] = "5. Você sabe como isso termina?";

                respostas[0] = "Porque algumas lembranças despertam apenas quando encontram aquilo que as feriu.";
                respostas[1] = "Seus ecos ainda perduram. Mas eles continuam distantes… como um eco atrás de uma porta fechada."; //valida aqui PMI
                respostas[2] = "Uma indagação válida… Não tenho conhecimento o bastante para soluciona-la, mas é engraçado, não eres o primeiro a fazer tal";
                respostas[3] = "Bom… Existem exceções, afinal, você viu um demônio nessa torre, mas, teoricamente, sim.";
                respostas[4] = "Em tese sei, mas apenas em tese, nunca lutei contra o Intervalo, eu faço parte dele.";

                break;

            case Andar5: // depois do Iowa
                num_perguntas = 6;

                opcoes_dialogo[0] = "1. O que existe no topo?";
                opcoes_dialogo[1] = "2. Você já chegou perto do topo?";
                opcoes_dialogo[2] = "3. O que acontece quando alguém sai daqui?";
                opcoes_dialogo[3] = "4. Por que você me ajuda?";
                opcoes_dialogo[4] = "5. Ainda existe algo me esperando fora daqui?";
                opcoes_dialogo[5] = "6. E se eu não gostar do que encontrar?";

                respostas[0] = "O que você mais sonha, teoricamente.";
                respostas[1] = "Perto o bastante para ouvir o chamado. Longe o bastante para nunca alcançá-lo.";
                respostas[2] = "Em tese, seria o retorno ao mundo… Em tese";
                respostas[3] = "Eu ajudo todos que passam por aqui, vejo que o potencial das almas aumenta quando recebe apoio.";
                respostas[4] = "Talvez. Um lugar. Uma voz. Ou alguém que ainda se recuse a esquecer você.";
                respostas[5] = "Então finalmente verá a verdade sem desejar tê-la encontrado.";

                break;
                
                bool conversando = true;
                while (conversando)
                {
                    werase(ponto_save_window);
                    box(ponto_save_window, 0, 0);
                    desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                    mvwprintw(ponto_save_window, 28, 6, "[Vigia]");

                    for (int i = 0; i < num_perguntas; i++)
                    {
                        if (ja_realizou_pergunta[i])
                            wattron(ponto_save_window, COLOR_PAIR(COR_OPCAO_INVALIDA));
                        mvwprintw(ponto_save_window, 30 + i, 6, opcoes_dialogo[i]);
                        wattrset(ponto_save_window, A_NORMAL);
                    }
                    wattron(ponto_save_window, COLOR_PAIR(COR_DESTAQUE));
                    mvwprintw(ponto_save_window, getmaxy(ponto_save_window) - 3, 4, "Pressione ENTER para se despedir");
                    wattroff(ponto_save_window, COLOR_PAIR(COR_DESTAQUE));
                    wrefresh(ponto_save_window);

                    opcao = wgetch(ponto_save_window);
                    int opcao_selecionada;
                    if (opcao == KEY_ENTER || opcao == '\n' || opcao == 10)
                    {
                        conversando = false;
                        break;
                    }
                    if (opcao == '1')
                        opcao_selecionada = 0;
                    if (opcao == '2')
                        opcao_selecionada = 1;
                    if (opcao == '3')
                        opcao_selecionada = 2;

                    if (!ja_realizou_pergunta[opcao_selecionada])
                    {
                        ja_realizou_pergunta[opcao_selecionada] = 1;
                        werase(ponto_save_window);
                        box(ponto_save_window, 0, 0);
                        desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                        mvwprintw(ponto_save_window, 28, 6, "[%s]", player->nome);
                        slow_mvwprintw(ponto_save_window, opcoes_dialogo[opcao_selecionada], 29, 6, 30);

                        wrefresh(ponto_save_window);
                        napms(1200);
                        mvwprintw(ponto_save_window, 30, 6, "[Vigia]");
                        slow_mvwprintw(ponto_save_window, respostas[opcao_selecionada], 31, 6, 30);

                        wrefresh(ponto_save_window);

                        if (opcao == KEY_ENTER || opcao == '\n' || opcao == 10)
                        {
                            conversando = false;
                            break;
                        }
                    }
                }
                werase(ponto_save_window);
                box(ponto_save_window, 0, 0);
                desenhar_sprite(ponto_save_window, "assets/sprites/others/vigia.txt", 5, 5);
                mvwprintw(ponto_save_window, 28, 6, "[%s]", player->nome);
                wrefresh(ponto_save_window);

                slow_mvwprintw(ponto_save_window, "Bem, vou voltar a Torre… Até", 29, 6, 40);

                break;

            case '4':
                slow_mvwprintw(ponto_save_window, "Você levanta e volta a Torre", getmaxy(ponto_save_window) - 3, 5, 30);
                descansando = false;
                break;
            }
        }
    }
}