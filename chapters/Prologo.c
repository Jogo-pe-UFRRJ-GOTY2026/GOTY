//Dialogo, e tutorial de combate
#include <stdbool.h>
#include <ncurses.h>

#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"

Player* Prologo()
{

    /// bla bla bla bla bla

    WINDOW* criacao_personagem_window = newwin(0,0,0,0);
    char nome[100];
    char *choices[] = {
        "Masculino",
        "Feminino",
        "Outro"
    };
    int highlighted_option = 0; // Current selection index
    int choice = -1;

    echo();                 // Permitiria os caracteres a aparecerem na tela

    addstr("Entre com seu nome: "); // vai ter que ser outra funcao aqui
    getnstr(nome, 99);     // Read string safely (leave space for \0)


    printw("Use as setas para selecionar o genero \n");

    
}