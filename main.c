#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include "objects/Player.h"
#include "chapters/Capitulos.h"
#include "system/Save.h"
#include "utils/utils.h"

// make :  compila o codigo
// make run : roda o codigo
// make clean: exclui o executavel e limpa o terminal
void menu_inicial(bool *deveria_carregar_salvamento);

int main()
{
    setlocale(LC_ALL, "");      // Permite printar caracteres unicode

    Player *player;
    int AndarSalvo;

    initscr();
    cbreak();                   //le a tecla sem precisar do enter
    noecho();                   // a tecla digitada é lida mas nao aparece no terminal
    keypad(stdscr, TRUE);       //libera as teclas especiais e as setinhas
    curs_set(0);                //
    nodelay(stdscr, FALSE);     //  configura a tela padrão do ncurses (stdscr) para input nao bloqueante
    //timeout(50);              // Usar timeout(50) configura getch() para esperar 50 milisegundos. Se nenhuma tecla é apertada em 50ms, getch() returna ERR.
    start_color();              // iniciliaza cores para aplicação
    use_default_colors();

    iniciar_cores(); // Inicializa os pares de cores definidos na funcao
    
    char tecla;
    bool deveria_carregar_salvamento = false;

    menu_inicial(&deveria_carregar_salvamento);


    // if(deveria_carregar_salvamento)
    // {
    //         player = carregar_salvamento();
    //         //AndarSalvo = player->NumeroAndar;
    // }
    // else 
    // {
    //     Player* player = NULL;
    //     AndarSalvo=0;
    // }


    // Não sei como vou fazer isso ainda
    // switch(AndarSalvo)
    // {
    //     case Andar0:
    //         player = Prologo(tecla);
    //     case Andar1:
    //     case Andar2:
    //     case Andar3:
    //         Capitulo1(player, tecla);
    //     case Andar4:
    //     case Andar5:
    //     case Andar6:
    //         Capitulo2(player, tecla);
    //     case Andar7:
    //     case Andar8:
    //     case Andar9:
    //         Capitulo3(player, tecla);
    //     case Andar10:
    //         Capitulo4(player, tecla);
    // }

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
    keypad(menu_win,TRUE); // habilita as setinhas na tela do menu
    char *opcoes[TOTAL_OPCOES] = {"Iniciar Novo Jogo", "Continuar Jogo Salvo", "Abrir Configurações", "Fechar Jogo"};

    opcao_selecionada selecionada = INICIAR_JOGO_NOVO;

    const int MENU_Y = 12;
    const int ESPACAMENTO_ENTRE_OPCOES = 2;

    //Loop de atualização do estado do menu
    while(true)
    {
        werase(menu_win);              //limpa a matriz para nao ficar poluida
        const int TITULO_X = (largura_tela - 55) / 2;
        mvwprintw(menu_win, 3, TITULO_X, "  __ _  __ _ _ __ ___   ___ _ __   __ _ _ __ ___   ___ ");
        mvwprintw(menu_win, 4, TITULO_X ," / _` |/ _` | '_ ` _ \\ / _ \\ '_ \\ / _` | '_ ` _ \\ / _ \\");
        mvwprintw(menu_win, 5, TITULO_X ,"| (_| | (_| | | | | | |  __/ | | | (_| | | | | | |  __/");
        mvwprintw(menu_win, 6, TITULO_X ," \\__, |\\__,_|_| |_| |_|\\___|_| |_|\\__,_|_| |_| |_|\\___|");
        mvwprintw(menu_win, 7, TITULO_X ," |___/  ");


        /* ============================= Imprimindo as opções ===============================================*/
        for(int opcao = 0; opcao < TOTAL_OPCOES;opcao++)
        {
            int y = MENU_Y + (opcao*ESPACAMENTO_ENTRE_OPCOES);
            int x = (largura_tela - strlen(opcoes[opcao]))/2;  //Pegando o centro da tela, relativo ao centro do texto, nao questione o calculo

            if(opcao==selecionada)
            {
                x = (largura_tela - strlen(opcoes[opcao]) + 2) / 2; // recalcula para considerar os ⦗    ⦘
                wattron(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Ativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(menu_win, y, x - 4, "➢"); // 4 Caracteres antes do texto
                mvwprintw(menu_win, y, x, "⦗%s⦘", opcoes[opcao]);
                wattroff(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Desativa um atributo na janela passada, nesse caso, a cor e o negrito
            }
            else
            {
                mvwprintw(menu_win,y,x,"%s", opcoes[opcao]);
            }
        }
        wrefresh(menu_win); // Atualiza a tela com as opções

        tecla = wgetch(menu_win); // pega o caracter
        switch(tecla)
        {
            case KEY_UP:
                if(selecionada==INICIAR_JOGO_NOVO) { selecionada=FECHAR_JOGO; }
                else { selecionada-=1; }
                break;

            case KEY_DOWN:
                if (selecionada == FECHAR_JOGO) { selecionada = INICIAR_JOGO_NOVO; }
                else { selecionada += 1; }
                break;

            case 10:        //Dependendo do sistema ou teclado ou sla, o enter pode retornar 10
            case KEY_ENTER:
                switch (selecionada)
                {
                    case INICIAR_JOGO_NOVO:
                        *deveria_carregar_salvamento = false;
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

                        napms(500); //espera meio segundo
                        selecionada=INICIAR_JOGO_NOVO;
                        
                        break;

                    case FECHAR_JOGO:

                        char aviso_encerramento[] = "《 Fechando jogo em 3 segundos 》";
                        wattron(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD);

                        mvwprintw(menu_win, altura_tela - 10, (largura_tela - strlen(aviso_encerramento)) / 2, "%s", aviso_encerramento);

                        wattroff(menu_win, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); 
                        wrefresh(menu_win);

                        napms(3000);       //espera 2 segundos


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





