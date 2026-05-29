#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include "tests.h"
#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"
#include <time.h>
// Uma breve aula para vocês senhores, um hábito muito bom de se criar em desenvolvimento de sistemas: Testes
// É prática existente na industria escrever testes para o seu sistema, para previnir erros durante as suas pipelines CI/CD e deploys incorretos
// Testes normalmente podem ser separados em alguns grupos, em ordem de custo e trabalho de implementar:
// 1. Unit Tests: Testa componentes individuais do codigo. Em python há bibliotecas como pytest e unittest que facilitam isso,
// No springboot do java há anotações que facilitam isso como @SpringBootTest @AutoConfigureMockMvc @Test
// 2. Integration Test: Testam o comportamento e comunicação dos componentes e serviços do sistema trabalhando juntos
// 3. System Testing: verifica se o sistema inteiro ta funcional
// 4. End to end testing -> Ignoramos esse até que seja absolutamente

int main()
{
    test_saving_system();
    printf("Teste do sistema de salvamento passados com sucesso");

    napms(2000);

    setlocale(LC_ALL, ""); 
    srand(time(NULL));    

    initscr();
    cbreak();               
    noecho();               
    keypad(stdscr, TRUE);   
    curs_set(0);            
    nodelay(stdscr, FALSE); 
    start_color(); 
    use_default_colors();
    iniciar_cores();
    
    //desenhar_sprite(stdscr, "assets/sprites/others/vigia.txt", 10,5);
    //getch();
    simular_combate();

    return 0;
}



