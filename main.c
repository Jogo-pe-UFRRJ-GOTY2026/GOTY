#include <stdio.h>
#include <ncurses.h>


// make :  compila o codigo
// make run : roda o codigo
// make clean: exclui o executavel e limpa o terminal
int main()
{
    
    initscr();
    cbreak();                   //le a tecla sem precisar do enter
    noecho();                   // a tecla digitada é lida mas nao aparece no terminal
    keypad(stdscr, TRUE);       //libera as teclas especiais e as setinhas
    curs_set(0);                //
    nodelay(stdscr, TRUE);      //  configura a tela padrão do ncurses (stdscr) para input nao bloqueante
    //timeout(50);              // Usar timeout(50) configura getch() para esperar 50 milisegundos. Se nenhuma tecla é apertada em 50ms, getch() returna ERR.
    start_color();              // iniciliaza cores para aplicação
    use_default_colors();
    char tecla;

    
    while(tecla!='q')
    {
        tecla = getch();
        
        refresh();
        napms(33); //quase 30 fps
    }
    endwin();
    return 0;
}