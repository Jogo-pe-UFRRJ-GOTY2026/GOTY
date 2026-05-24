#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../objects/Player.h"
#include "../objects/Inventario.h"
#include "../utils/utils.h"
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_OF_ELEMENTS 1
Player* carregar_salvamento()
{

    FILE *save_file = fopen("saves/player_save.bin", "rb");
    if(save_file==NULL)
    {
        //perror("Test Error: Erro ao ler arquivo de salvamento");
        return NULL;
    }

    Player* player = malloc(sizeof(Player));
    if(player==NULL)
    {
        //perror("Test Error: Erro ao alocar estrutura player");
        fclose(save_file);
        return NULL;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if(fread(player, sizeof(Player), NUM_OF_ELEMENTS, save_file)!=NUM_OF_ELEMENTS)
    {
        //perror("Test Error: Erro ao ler estrutura player do arquivo de salvamento");
        free(player);
        fclose(save_file);
        return NULL;
    }
    fclose(save_file);
    

    return player;
}

bool salvar_jogo(Player *player)
{
    mkdir("saves", 0777);
    FILE* save_file = fopen("saves/player_save.bin", "wb");
    if(save_file==NULL)
    {
        perror("Test Error: Erro ao criar arquivo de salvamento");
        return false;
    }

    // ponteiro para os dados, tamanho de cada elemento, quantidade de elementos, arquivo
    if(fwrite(player, sizeof(Player), NUM_OF_ELEMENTS, save_file) != NUM_OF_ELEMENTS)
    {
        perror("Test Error: Erro ao escrever estrutura player no arquivo de salvamento");
        fclose(save_file);
        return false;
    }
    fclose(save_file);
    return true;


}
