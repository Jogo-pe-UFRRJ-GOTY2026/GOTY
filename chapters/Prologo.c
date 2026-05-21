//Dialogo, e tutorial de combate
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"

Player* Prologo()
{
    // =====================================
    // INTRODUCAO
    // =====================================

    clear();

    mvprintw(5, 10, "==============================");
    mvprintw(6, 15, "ECHOBOUND");
    mvprintw(7, 10, "==============================");

    refresh();
    napms(2000);

    clear();

    mvprintw(10, 10, "No fim do mundo...");
    refresh();
    napms(2500);

    clear();

    mvprintw(10, 10, "restou uma torre.");
    refresh();
    napms(2500);

    clear();

    mvprintw(10, 10, "A Torre das Cinzas Quietas.");
    refresh();
    napms(3000);

    clear();

    mvprintw(10, 10, "Ninguem sabe quem a construiu.");
    refresh();
    napms(2500);

    clear();

    mvprintw(10, 10, "Ninguem sabe o que existe no topo.");
    refresh();
    napms(3000);

    clear();

    mvprintw(10, 10, "Mas ela continua chamando.");
    refresh();
    napms(3000);

    clear();

    mvprintw(10, 10, "E pessoas continuam respondendo.");
    refresh();
    napms(3500);

    clear();

    mvprintw(10, 10, "Voce chegou tarde.");
    refresh();
    napms(3000);

    clear();

    mvprintw(10, 10, "As brasas ja estao morrendo.");
    refresh();
    napms(3500);

    clear();

    // =====================================
    // CRIACAO PERSONAGEM
    // =====================================

    WINDOW* criacao_personagem_window = newwin(20, 60, 2, 10);

    char nome[100];

    char *choices[] = {
        "Masculino",
        "Feminino",
        "Outro"
    };

    int highlighted_option = 0;
    int tecla;

    Genero generoEscolhido = Masculino;

    keypad(criacao_personagem_window, TRUE);

    box(criacao_personagem_window, 0, 0);

    mvwprintw(criacao_personagem_window,
              1,
              2,
              "CRIACAO DE PERSONAGEM");

    wrefresh(criacao_personagem_window);

    echo();

    mvwprintw(criacao_personagem_window,
              4,
              2,
              "Entre com seu nome: ");

    wgetnstr(criacao_personagem_window,
             nome,
             99);

    noecho();

    while(true)
    {
        werase(criacao_personagem_window);

        box(criacao_personagem_window, 0, 0);

        mvwprintw(criacao_personagem_window,
                  1,
                  2,
                  "CRIACAO DE PERSONAGEM");

        mvwprintw(criacao_personagem_window,
                  4,
                  2,
                  "Nome: %s",
                  nome);

        mvwprintw(criacao_personagem_window,
                  6,
                  2,
                  "Use as setas para selecionar o genero");

        for(int i = 0; i < 3; i++)
        {
            if(i == highlighted_option)
                wattron(criacao_personagem_window, A_REVERSE);

            mvwprintw(criacao_personagem_window,
                      9 + i,
                      5,
                      "%s",
                      choices[i]);

            wattroff(criacao_personagem_window, A_REVERSE);
        }

        mvwprintw(criacao_personagem_window,
                  15,
                  2,
                  "ENTER para confirmar");

        wrefresh(criacao_personagem_window);

        tecla = wgetch(criacao_personagem_window);

        switch(tecla)
        {
            case KEY_UP:

                highlighted_option--;

                if(highlighted_option < 0)
                    highlighted_option = 2;

                break;

            case KEY_DOWN:

                highlighted_option++;

                if(highlighted_option > 2)
                    highlighted_option = 0;

                break;

            case 10:

                generoEscolhido = highlighted_option;
                goto personagem_criado;
        }
    }

personagem_criado:

    delwin(criacao_personagem_window);

    clear();

    // =====================================
    // ORIN
    // =====================================

    mvprintw(10, 10, "ORIN:");
    refresh();
    napms(1000);

    mvprintw(12, 10, "\"Outro viajante.\"");
    refresh();
    napms(2500);

    mvprintw(14, 10, "\"Fazia tempo.\"");
    refresh();
    napms(3000);

    clear();

    mvprintw(10, 10, "ORIN:");
    refresh();

    mvprintw(12, 10, "\"A maioria sobe procurando respostas.\"");
    refresh();
    napms(3500);

    mvprintw(14, 10, "\"Os inteligentes voltam antes do topo.\"");
    refresh();
    napms(3500);

    clear();

    mvprintw(10, 10, "ORIN:");
    refresh();

    mvprintw(12, 10, "\"Os outros...\"");
    refresh();
    napms(2500);

    mvprintw(14, 10, "\"...viram parte da torre.\"");
    refresh();
    napms(4000);

    clear();

    mvprintw(10, 10, "ORIN:");
    refresh();

    mvprintw(12, 10, "\"Se pretende continuar...\"");
    refresh();
    napms(3000);

    mvprintw(14, 10, "\"Aprenda quando lutar.\"");
    refresh();
    napms(3000);

    mvprintw(16, 10, "\"E quando mostrar misericordia.\"");
    refresh();
    napms(4000);

    clear();

    // =====================================
    // HOLLOW
    // =====================================

    mvprintw(10, 10, "Passos ecoam na escuridao.");
    refresh();
    napms(3500);

    clear();

    mvprintw(10, 10, "HOLLOW:");
    refresh();
    napms(1500);

    mvprintw(12, 10, "\"Voce parece assustado.\"");
    refresh();
    napms(3000);

    mvprintw(14, 10, "\"Isso e bom.\"");
    refresh();
    napms(3500);

    clear();

    // =====================================
    // PLAYER
    // =====================================

    Player* player = criar_player(nome, generoEscolhido);

    player->vida = 30;

    player->atributos.forca = 5;
    player->atributos.agilidade = 5;
    player->atributos.inteligencia = 5;
    player->atributos.vitalidade = 5;

    player->posicao.x = 10;
    player->posicao.y = 10;

    return player;
}