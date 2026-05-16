#include <stdio.h>
#include <ncurses.h>


// make :  compila o codigo
// make run : roda o codigo
// make clean: exclui o executavel e limpa o terminal
int main()
{
    
    initscr();
    cbreak(); //le a tecla sem precisar do enter
    noecho(); // a tecla digitada é lida mas nao aparece no terminal
    keypad(stdscr, TRUE); //libera as teclas especiais e as setinhas
    curs_set(0);

    char tecla;
    while(tecla!='q')
    {
        tecla = getch();
        
        refresh();
    }
    endwin();
    return 0;
}