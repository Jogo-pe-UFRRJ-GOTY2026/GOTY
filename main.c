#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "objects/Player.h"
#include "chapters/CAPITULO.h"
#include "system/Save.h"
#include "utils/utils.h"

// make :  compila o codigo
// make run : roda o codigo
// make clean: exclui o executavel e limpa o terminal
void menu_inicial(bool *deveria_carregar_salvamento);

int main()
{
    setlocale(LC_ALL, ""); // Permite printar caracteres unicode
    srand(time(NULL)); //randomiza a seed do jogo.

    Player *player;
    int AndarSalvo;

    initscr();
    cbreak();               // le a tecla sem precisar do enter
    noecho();               // a tecla digitada é lida mas nao aparece no terminal
    keypad(stdscr, TRUE);   // libera as teclas especiais e as setinhas
    curs_set(0);            //
    nodelay(stdscr, FALSE); //  configura a tela padrão do ncurses (stdscr) para input nao bloqueante
    // timeout(50);              // Usar timeout(50) configura getch() para esperar 50 milisegundos. Se nenhuma tecla é apertada em 50ms, getch() returna ERR.
    start_color(); // iniciliaza cores para aplicação
    use_default_colors();

    iniciar_cores(); // Inicializa os pares de cores definidos na funcao

    char tecla;
    bool deveria_carregar_salvamento = false;

    menu_inicial(&deveria_carregar_salvamento);
    if(deveria_carregar_salvamento)
    {
            player = carregar_salvamento();
            //AndarSalvo = player->NumeroAndar;
    }
    else
    {
        Player* player = NULL;
        AndarSalvo=0;
    }

    switch(AndarSalvo)
    {
        case Andar0:
            player = Prologo();
        case Andar1:
        case Andar2:
        case Andar3:
            Capitulo1(player);
        case Andar4:
        case Andar5:
        case Andar6:
            Capitulo2(player);
        case Andar7:
        case Andar8:
        case Andar9:
            Capitulo3(player);
        case Andar10:
            Capitulo4(player);
    }

    getch();

    endwin();
    return 0;
}

void menu_inicial(bool *deveria_carregar_salvamento)
{
    int tecla;
    int altura_tela, largura_tela;
    getmaxyx(stdscr, altura_tela, largura_tela); // retornam o comprimento e largura maxima do terminal no momento, respectivamente o maximo de linhas e colunas da matriz

    WINDOW *menu_win = newwin(altura_tela, largura_tela, 0, 0); // As duas ultimas coordenadas sao as de inicio, ou seja, o inicio da matriz[0][0]
    keypad(menu_win, TRUE);                                     // habilita as setinhas na tela do menu
    char *opcoes[TOTAL_OPCOES] = {"Iniciar Novo Jogo", "Continuar Jogo Salvo", "Abrir Configurações", "Fechar Jogo"};

    opcao_selecionada selecionada = INICIAR_JOGO_NOVO;

    const int MENU_Y = 12;
    const int ESPACAMENTO_ENTRE_OPCOES = 2;

    // Loop de atualização do estado do menu
    while (true)
    {
        werase(menu_win); // limpa a matriz para nao ficar poluida
        int titulo_largura = 70;
        int TITULO_X = (largura_tela - titulo_largura) / 2;

        mvwprintw(menu_win, 2, TITULO_X, "███████╗ ██████╗██╗  ██╗ ██████╗ ██████╗  ██████╗ ██╗   ██╗███╗   ██╗██████╗ ");
        mvwprintw(menu_win, 3, TITULO_X, "██╔════╝██╔════╝██║  ██║██╔═══██╗██╔══██╗██╔═══██╗██║   ██║████╗  ██║██╔══██╗");
        mvwprintw(menu_win, 4, TITULO_X, "█████╗  ██║     ███████║██║   ██║██████╔╝██║   ██║██║   ██║██╔██╗ ██║██║  ██║");
        mvwprintw(menu_win, 5, TITULO_X, "██╔══╝  ██║     ██╔══██║██║   ██║██╔══██╗██║   ██║██║   ██║██║╚██╗██║██║  ██║");
        mvwprintw(menu_win, 6, TITULO_X, "███████╗╚██████╗██║  ██║╚██████╔╝██████╔╝╚██████╔╝╚██████╔╝██║ ╚████║██████╔╝");
        mvwprintw(menu_win, 7, TITULO_X, "╚══════╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═════╝ ");

        /* ============================= Imprimindo as opções ===============================================*/
        for (int opcao = 0; opcao < TOTAL_OPCOES; opcao++)
        {
            int y = MENU_Y + (opcao * ESPACAMENTO_ENTRE_OPCOES);
            int x = (largura_tela - strlen(opcoes[opcao])) / 2; // Pegando o centro da tela, relativo ao centro do texto, nao questione o calculo

            if (opcao == selecionada)
            {
                wattron(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Ativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(menu_win, y, x - 6, "➢");                            // 6 Caracteres antes do texto
                mvwprintw(menu_win, y, x - 2, "⦗");                            // 6 Caracteres antes do texto
                mvwprintw(menu_win, y, x, "%s", opcoes[opcao]);
                mvwprintw(menu_win, y, x + strlen(opcoes[opcao]), " ⦘");                             // 6 Caracteres antes do texto
                wattroff(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Desativa um atributo na janela passada, nesse caso, a cor e o negrito
            }
            else
            {
                mvwprintw(menu_win, y, x, "%s", opcoes[opcao]);
            }
        }
        wrefresh(menu_win); // Atualiza a tela com as opções

        tecla = wgetch(menu_win); // pega o caracter
        switch (tecla)
        {
        case KEY_UP:
            if (selecionada == INICIAR_JOGO_NOVO)
            {
                selecionada = FECHAR_JOGO;
            }
            else
            {
                selecionada -= 1;
            }
            break;

        case KEY_DOWN:
            if (selecionada == FECHAR_JOGO)
            {
                selecionada = INICIAR_JOGO_NOVO;
            }
            else
            {
                selecionada += 1;
            }
            break;

        case 10: // Dependendo do sistema ou teclado ou sla, o enter pode retornar 10
        case KEY_ENTER:
            switch (selecionada)
            {
            case INICIAR_JOGO_NOVO:
                *deveria_carregar_salvamento = false;

                // Player = Prologo(); ???
                
                delwin(menu_win);
                return;
            case CONTINUAR_JOGO:
                *deveria_carregar_salvamento = true;
                delwin(menu_win);
                return;
            case ABRIR_CONFIGURACOES:
                char aviso_configuracoes[] = "《 Menu de configurações não implementado 》";
                wattron(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);

                mvwprintw(menu_win, altura_tela - 10, (largura_tela - strlen(aviso_configuracoes)) / 2, "%s", aviso_configuracoes);

                wattroff(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);
                wrefresh(menu_win);

                napms(500); // espera meio segundo
                selecionada = INICIAR_JOGO_NOVO;

                break;

            case FECHAR_JOGO:

                wattron(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);

                for (int i = 3; i >= 1; i--) {
                    char aviso_encerramento[50];
                    sprintf(aviso_encerramento, "《 Fechando jogo em %d segundos 》", i);

                    // limpa a linha anterior
                    wmove(menu_win, altura_tela - 10, 0); //reposiciona o cursor
                    wclrtoeol(menu_win);                  //apaga do cursor ate o fim da lina end of line

                    // imprime centralizado
                    mvwprintw(menu_win, altura_tela - 10, (largura_tela - strlen(aviso_encerramento)) / 2, "%s", aviso_encerramento);

                    wrefresh(menu_win);

                    napms(1000);
                }

                wattroff(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);

                delwin(menu_win);
                endwin();

                return;
            default:
                break;
            }

            break;
        }
    }

    delwin(menu_win);
}