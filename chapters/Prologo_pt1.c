// Dialogo, e tutorial de combate
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "CAPITULO.h"
#include "../objects/Player.h"
#include "../objects/Inimigo.h"
#include "../objects/Inventario.h"
#include "../utils/utils.h"

void Introducao();

#define print_descricao(win, string, y, x, delay)       \
    ({                                                  \
        tecla = wgetch(win);                            \
        if(tecla== KEY_ENTER || tecla == 10)            \
        {                                               \
            werase(win);                                \
            wrefresh(win);                              \
            delwin(win);                                \
            win=NULL;                                   \
            break;                                      \
        }                                               \
        slow_mvwprintw(win, string, y,x,delay);         \
        tecla=wgetch(win);                              \
        if(tecla== KEY_ENTER || tecla == 10)            \
        {                                               \
            werase(win);                                \
            wrefresh(win);                              \
            delwin(win);                                \
            win=NULL;                                   \
            break;                                      \
        }                                               \
    })

Player* Prologo_pt1()
{

    Introducao();
    Player* player = Tela_criacao();

    WINDOW *tela_descricao = newwin(getmaxy(stdscr)-10, getmaxx(stdscr)-20, 0, 10);
    keypad(tela_descricao, TRUE);
    nodelay(tela_descricao, TRUE);
    box(tela_descricao,0,0);
    int tecla;
    // isso fica em baixo
    

    for (int x = 1, y = 16; x < getmaxx(tela_descricao)-1; x++)
        mvwprintw(tela_descricao, y, x, "-");

    wattron(tela_descricao, COLOR_PAIR(COR_DESTAQUE));
    mvwprintw(tela_descricao,18,35, "Aperte Enter para se aproximar da Torre dos Echos.");
    wattroff(tela_descricao, COLOR_PAIR(COR_DESTAQUE));


    // FODA-SE, O JOGADOR NÃO VAI PULAR ISSO, APRENDA A LER CARALHO, ANLAFABETISMO DA POPULAÇÃO TEM QUE DIMINUIR DE TODO MODO
    do{
    print_descricao(tela_descricao,player->nome,1,1,30);
    print_descricao(tela_descricao,"... Uma alma marcada pela determinação.",1, 1+strlen(player->nome),30);
    print_descricao(tela_descricao,"Alguém que carregava ambições impossíveis… e um desejo tão profundo que se tornou a última parte intacta de sua existência :",2,1,30);
    
    wattron(tela_descricao, A_BOLD);
    print_descricao(tela_descricao, player->genero==Masculino ? "Ser lembrado." : "Ser lembrada.",3,5,30);
    wattroff(tela_descricao, A_BOLD);

    print_descricao(tela_descricao, player->nome,5,1,30);
    print_descricao(tela_descricao, " é uma das almas pertencentes ao Intervalo, sofrendo por mal se lembrar de si mesmo e dos feitos em vida, ", 5, 1 + strlen(player->nome), 30);
    print_descricao(tela_descricao, "e sem ter como acabar com esse sofrimento lento de perda de significado próprio", 6, 1, 30);

    print_descricao(tela_descricao, "Afinal, a Morte, Coletora das almas mortas, o pega e traz para o único lugar sua alma pode ficar. ", 7, 1, 30);
    print_descricao(tela_descricao,"No Intervalo, perpetuando esse ciclo sem saída, ou quase).",8,1,30);


    print_descricao(tela_descricao, "A sua alma determinada por reconhecimento ainda exala o desejo de estar viva e se lembrar do indivíduo", 10, 1, 30);
    print_descricao(tela_descricao, "que o mantém em suas memórias que o impedem de perder sua essência.", 11, 1, 30);

    print_descricao(tela_descricao, "Porém, a alma de ", 12, 1, 30);
    print_descricao(tela_descricao, player->nome, 12, 18, 30);
    print_descricao(tela_descricao, " está se apagando…, será que o indivíduo se esqueceu definitivamente dessa alma?", 12, 19+ strlen(player->nome), 30);
    print_descricao(tela_descricao, "Será que o indivíduo morreu?", 13, 1 , 30);
    print_descricao(tela_descricao, "Com essas dúvidas, e a descoberta de uma possibilidade de sair daquele lugar, ", 14, 1, 30);
    print_descricao(tela_descricao, player->nome, 14, 78, 30);
    print_descricao(tela_descricao, "toma uma atitude…", 14, 79 + strlen(player->nome), 30);
    } while(0);

    while(tela_descricao != NULL)
    {
        tecla = wgetch(tela_descricao);
        if (tecla == KEY_ENTER || tecla == '\n' || tecla == 10)
        {
            werase(tela_descricao);
            wrefresh(tela_descricao);
            delwin(tela_descricao);
            break;
        }
    }

    WINDOW* tela_encontro_vigia = newwin(getmaxy(stdscr), getmaxx(stdscr),0,0);
    int ja_realizou_pergunta[3]={0};
    desenhar_sprite(tela_encontro_vigia, "assets/sprites/others/vigia.txt", 1,1);
    wrefresh(tela_encontro_vigia);

    char *dialogos[3] = {
        "Me chamam de muitas coisas, observador,sentinela…"
        "Mas você pode me chamar de Vigia, mais uma alma como você. E você é?",
        "Possivelmente, milhares de almas almejam subir a torre, a gente já deve ter se visto alguma outra vez,"
        "mas você ainda não fez nada suficientemente impactante para ficar marcado na minha memória…",

        "Conforme você avança na torre, cada chefe que você derrota é uma conquista digna de um eco,"
        "e cada eco atinge a memória de milhares de almas do intervalo, que quando reunidas, equivalem à memória de um ser humano."
        "Se prepare, se você quiser encher seu medidor, vai precisar de muitas almas para substituir 7 simples vivos."
    };

    

    napms(10000);


    apagar_janela(tela_encontro_vigia);



    player->NumeroAndar = 0;
    return player;
}   





// ================================================================================= //
//                              Criação de personagem                                // 
// ================================================================================= //
Player* Tela_criacao()
{
    int altura_tela, largura_tela;
    getmaxyx(stdscr, altura_tela, largura_tela);

    WINDOW *criacao_personagem_window = newwin(0, 0, 0, 0);

    char nome[50] = "";
    int tamanho_atual_nome = 0;
    char *opcoes[] = {"Masculino", "Feminino"};
    int largura_box = largura_tela / 3;
    int start_x = 5;
    box(criacao_personagem_window, 0, 0);

    int genero_selecionado = Masculino;
    int tecla;
    keypad(criacao_personagem_window, TRUE);
    echo(); // Permitiria os caracteres a aparecerem na tela, de

    while (true)
    {
        werase(criacao_personagem_window);

        // ====================================================== TITULO =====================================================

        mvwprintw(criacao_personagem_window, 3, largura_tela / 3, "██████╗██████╗ ██╗ █████╗  ██████╗ █████╗  ██████╗     ██████╗ ███████╗");
        mvwprintw(criacao_personagem_window, 4, largura_tela / 3, "██╔════╝██╔══██╗██║██╔══██╗██╔════╝██╔══██╗██╔═══██╗    ██╔══██╗██╔════╝");
        mvwprintw(criacao_personagem_window, 5, largura_tela / 3, "██║     ██████╔╝██║███████║██║     ███████║██║   ██║    ██║  ██║█████╗");
        mvwprintw(criacao_personagem_window, 6, largura_tela / 3, "██║     ██╔══██╗██║██╔══██║██║     ██╔══██║██║   ██║    ██║  ██║██╔══╝ ");
        mvwprintw(criacao_personagem_window, 7, largura_tela / 3, "╚██████╗██║  ██║██║██║  ██║╚██████╗██║  ██║╚██████╔╝    ██████╔╝███████╗");
        mvwprintw(criacao_personagem_window, 8, largura_tela / 3, " ╚═════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝     ╚═════╝ ╚══════╝");

        mvwprintw(criacao_personagem_window, 3, largura_tela / 3, "██████╗██████╗ ██╗ █████╗  ██████╗ █████╗  ██████╗     ██████╗ ███████╗");   
        mvwprintw(criacao_personagem_window, 4, largura_tela / 3, "██╔════╝██╔══██╗██║██╔══██╗██╔════╝██╔══██╗██╔═══██╗    ██╔══██╗██╔════╝");  
        mvwprintw(criacao_personagem_window, 5, largura_tela / 3, "██║     ██████╔╝██║███████║██║     ███████║██║   ██║    ██║  ██║█████╗"); 
        mvwprintw(criacao_personagem_window, 6, largura_tela / 3, "██║     ██╔══██╗██║██╔══██║██║     ██╔══██║██║   ██║    ██║  ██║██╔══╝ ");    
        mvwprintw(criacao_personagem_window, 7, largura_tela / 3, "╚██████╗██║  ██║██║██║  ██║╚██████╗██║  ██║╚██████╔╝    ██████╔╝███████╗");  
        mvwprintw(criacao_personagem_window, 8, largura_tela / 3, " ╚═════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝     ╚═════╝ ╚══════╝");    

        mvwprintw(criacao_personagem_window, 10, largura_box, "██████╗ ███████╗██████╗ ███████╗ ██████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗███╗   ███╗");
        mvwprintw(criacao_personagem_window, 11, largura_box, "██╔══██╗██╔════╝██╔══██╗██╔════╝██╔═══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝████╗ ████║");
        mvwprintw(criacao_personagem_window, 12, largura_box, "██████╔╝█████╗  ██████╔╝███████╗██║   ██║██╔██╗ ██║███████║██║  ███╗█████╗  ██╔████╔██║");
        mvwprintw(criacao_personagem_window, 13, largura_box, "██╔═══╝ ██╔══╝  ██╔══██╗╚════██║██║   ██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██║╚██╔╝██║");
        mvwprintw(criacao_personagem_window, 14, largura_box, "██║     ███████╗██║  ██║███████║╚██████╔╝██║ ╚████║██║  ██║╚██████╔╝███████╗██║ ╚═╝ ██║");
        mvwprintw(criacao_personagem_window, 15, largura_box, "╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝");

        mvwprintw(criacao_personagem_window, 20, start_x, "+------------------------------------------------------------+");
        mvwprintw(criacao_personagem_window, 21, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 22, start_x, "|   Digite seu nome: %s", nome);
        mvwprintw(criacao_personagem_window, 22, start_x + 61, "|");
        mvwprintw(criacao_personagem_window, 23, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 24, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 25, start_x, "|   Genero:                                                  |");

        // =====================================================
        // OPCOES GENERO
        // =====================================================
        for (int i = 0, y = 26; i < 2; i++, y++)
        {
            if (i == genero_selecionado)
            {
                mvwprintw(criacao_personagem_window, y, start_x, "|");
                wattron(criacao_personagem_window, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Ativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(criacao_personagem_window, y, start_x + 1, "     ➢  ⦗%s⦘", opcoes[i]);
                wattroff(criacao_personagem_window, COLOR_PAIR(COR_OPCAO_SELECIONADA) | A_BOLD); // Desativa um atributo na janela passada, nesse caso, a cor e o negrito
                mvwprintw(criacao_personagem_window, y, start_x + 61, "|");
            }
            else
            {
                mvwprintw(criacao_personagem_window, y, start_x, "|         %s ", opcoes[i]);
                mvwprintw(criacao_personagem_window, y, start_x + 61, "|");
            }
        }          
        mvwprintw(criacao_personagem_window, 28, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 29, start_x, "| Enter para confirmar                                       |");
        mvwprintw(criacao_personagem_window, 30, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 31, start_x, "|                                                            |");
        mvwprintw(criacao_personagem_window, 32, start_x, "+------------------------------------------------------------+");

        // Desenhando o sprite

        switch (genero_selecionado)
        {
        case Masculino:
            desenhar_sprite(criacao_personagem_window, "assets/sprites/player/sprite_masculino_terminalfix.txt", 17, 85);
            break;
        case Feminino:
            desenhar_sprite(criacao_personagem_window, "assets/sprites/player/sprite_feminino_terminalfix.txt", 17, 85);
            break;
        default:
            break;
        }


        wrefresh(criacao_personagem_window);

        // TRATANDO DO INPUT
        tecla = wgetch(criacao_personagem_window); // pega o caracter
        if (tecla == 10 || tecla == KEY_ENTER)

        {
            werase(criacao_personagem_window);
            wrefresh(criacao_personagem_window);
            break;
        }
        else if (tecla == KEY_DOWN)
        {
            if (genero_selecionado == Feminino)
                genero_selecionado = Masculino;
            else
                genero_selecionado += 1;
        }
        else if (tecla == KEY_UP)
        {
            if (genero_selecionado == Masculino)
                genero_selecionado = Feminino;
            else
                genero_selecionado -= 1;
        }
        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8)
        {
            if (tamanho_atual_nome > 0)
            {
                tamanho_atual_nome -= 1;
                nome[tamanho_atual_nome] = '\0';
            }
        }
        else if (' ' <= tecla && tecla <= '~')
        {
            if (tamanho_atual_nome < sizeof(nome) - 1)
            {
                nome[tamanho_atual_nome] = tecla;
                tamanho_atual_nome += 1;
                nome[tamanho_atual_nome] = '\0';
            }
        }
    }

    delwin(criacao_personagem_window);
    Player *player = criar_player(nome, genero_selecionado);

    return player;
}


// ================================================================================= //
//                              Introdução da história                               //
// ================================================================================= //
void Introducao()
{
    int altura, largura;
    getmaxyx(stdscr, altura, largura);

    WINDOW *tela_apresentacao = newwin(0, 0, 0, 0);
    nodelay(tela_apresentacao, TRUE);
    keypad(tela_apresentacao, TRUE);

    int esquerda = 25, direita = 140;
    int topo = 5, base = 32;
    int tecla;
    char *texto[] = {
        "Após a morte, existem 4 regiões.",
        "Paraíso, onde as boas almas vão.",
        "Purgatório, onde você se arrepende de seus pecados.",
        "Inferno, onde as almas ruins vão.",
        "E o Intervalo, uma região onde qualquer alma esquecida se encaminha.",
        "Lá, as almas sofrem uma perda de memória progressiva,",
        "Até se tornarem seres vagando sem lembranças, quando ninguém mais se lembra delas.",
        "No Intervalo, existe uma torre chamada Torre dos Ecos,",
        "Ela oferece às almas esquecidas uma chance de serem lembradas."};
    int num_dialogos = sizeof(texto) / sizeof(texto[0]);

    for (int i = 0; i < num_dialogos; i++)
    {
        werase(tela_apresentacao);


        wattron(tela_apresentacao, COLOR_PAIR(COR_DESTAQUE) | A_BOLD); // Ativa um atributo na janela passada, nesse caso, a cor e o negrito
        mvwprintw(tela_apresentacao, base + 2, direita-30, "Pressione ENTER para pular");
        wattroff(tela_apresentacao, COLOR_PAIR(COR_DESTAQUE) | A_BOLD); // Desativa um atributo na janela passada, nesse caso, a cor e o negrito

            

        for (int x = esquerda + 1; x < direita; x++)
            mvwprintw(tela_apresentacao, topo, x, "-");
        for (int x = esquerda + 1; x < direita; x++)
            mvwprintw(tela_apresentacao, base, x, "-");

        desenhar_sprite(tela_apresentacao, "assets/sprites/buildings/tower.txt", topo + 1, esquerda + 1);

        for (int y = topo; y <= base; y++)
        {
            if (y == topo || y == base)
            {
                mvwprintw(tela_apresentacao, y, esquerda, "+");
                mvwprintw(tela_apresentacao, y, direita, "+");
            }
            else
            {
                mvwprintw(tela_apresentacao, y, esquerda, "|");
                mvwprintw(tela_apresentacao, y, direita, "|");
            }
        }
        wrefresh(tela_apresentacao);

        int metade_box = esquerda + (direita - esquerda) / 2 - 5; //vide, o meio visual é meio logico - 5 pq sim
        slow_mvwprintw(tela_apresentacao, texto[i], 2, metade_box - strlen(texto[i])/2, 50);

        tecla = wgetch(tela_apresentacao);
        if (tecla == KEY_ENTER || tecla == 10)
            break;
        wrefresh(tela_apresentacao);
        napms(2000); //espera 2 segundos
        tecla = wgetch(tela_apresentacao);
        if (tecla == KEY_ENTER || tecla == 10)
            break;
    }
    
    delwin(tela_apresentacao);




}