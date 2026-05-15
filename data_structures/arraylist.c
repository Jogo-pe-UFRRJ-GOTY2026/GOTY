#include "arraylist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*===============================================================================
    |   |   |‾‾‾   |     |‾‾‾\  |‾‾‾  |‾‾‾\   /‾‾‾\
    |___|   |___   |     |___/  |___  |___/   \___
    |   |   |      |     |      |     |  \        \
    |   |   |___   |___  |      |___  |   \   \___/
===============================================================================*/

typedef char *first_element_address;
first_element_address get_first_element_address(ArrayList *arrayList)
{
    return (first_element_address)arrayList->data; // posição de memoria do primeiro elemento, em chars pois é 1 byte e facilita o calculo
}
// Inline faz o codigo da função ser escrito direto onde é chamado, evitando o pulo e salvando o tempo de chamar uma função separada
// Static previne a função de ser vista fora do arquivo



// calcula a posição na memoria do elemento a ser adicionado, endereço inicial + nº bytes até o elemento adicionado
static inline void *get_element_position(first_element_address base, int position, size_t element_size)
{
    return base + position * element_size;
}

void resize_list(ArrayList *arrayList)
{
    arrayList->capacity = arrayList->capacity*3 / 2 + 1; 
    void *temp = realloc(arrayList->data, arrayList->capacity * arrayList->element_size);
    if (temp == NULL)
    {
        exit(1);
    }
    arrayList->data = temp;
}


/*===============================================================================
    |‾‾‾  |   |  |\   |   /‾‾‾‾   ‾‾|‾‾  O   /‾‾‾\  |\   |  /‾‾‾\
    |___  |   |  | \  |  |          |    |   |   |  | \  |  \___
    |     |   |  |  \ |  |          |    |   |   |  |  \ |      \
    |     |___|  |   \|   \____     |    |   \___/  |   \|  \___/
===============================================================================*/



ArrayList *arraylist_new(size_t element_size)
{
    ArrayList *ptr = (ArrayList*) malloc(sizeof(ArrayList));
    if (ptr == NULL)
    {
        exit(1);
    }
    ptr->capacity = 8;
    ptr->length = 0;
    ptr->element_size = element_size;
    ptr->data = malloc(ptr->capacity * element_size);
    if(ptr->data == NULL)
    {
        exit(1);
    }
    return ptr; 
}

ArrayList *arraylist_new_from(void *array, size_t length, size_t capacity, size_t element_size)
{
    if (capacity == 0)
    {
        capacity = 8;
    }
    if (length > capacity || array == NULL)  //garante que nao incializa uma arraylist com capacidade invalida
    {
        return NULL;
    }
    ArrayList *ptr = (ArrayList *)malloc(sizeof(ArrayList));
    if (ptr == NULL)
    {
        exit(1);
    }

    ptr->length = length;
    ptr->capacity = capacity;
    ptr->element_size = element_size;
    ptr->data = malloc(ptr->capacity * element_size);
    if (ptr->data == NULL)
    {
        exit(1);
    }

    memcpy(ptr->data,array, ptr->length*ptr->element_size);
    

    return ptr;
}



void _arraylist_append(ArrayList *arrayList, void *element)
{

    if(arrayList->length >= arrayList->capacity)     // checa se isso passa da capacidade dela e aumenta em 1.5x, evitando alocar altas quantidades
    {    
        resize_list(arrayList);
    }
    first_element_address base = get_first_element_address(arrayList); // posição inicial da array


    memcpy(get_element_position(base, arrayList->length, arrayList->element_size), element, arrayList->element_size);
    arrayList->length +=1; // aumentar o atributo comprimento da array
}




void* _arraylist_pop(ArrayList *arrayList, int index)
{

    if(index>=arrayList->length || index<0) //se ele tentar tirar um elemento numa posição nao preenchida
    {
        return NULL;
    }

    void* popped_element_position = _arraylist_get_element_address(arrayList, index);

    void* popped_element_cpy = malloc(arrayList->element_size);
    memcpy(popped_element_cpy,popped_element_position, arrayList->element_size); //copio o elemento que será excluido

    void* next_element = _arraylist_get_element_address(arrayList, index+1); //pego o proximo elemento
    arrayList->length-=1; //diminui o comprimento da arraylist

    memmove(popped_element_position, next_element, (arrayList->length - index) * arrayList->element_size);
    // colocando o destino como a partir do endereço de memoria do elemento retirado, 
    // pegando os elementos apos ele (index+1), e o numero de bytes a serem movidos
    // Assim um pop(5) numa array deve tacar os elementos começando no indice 5 1 posição para tras  


    return popped_element_cpy;
}


int _arraylist_indexOf(ArrayList *arrayList, void* element)
{

    for (int index = 0; index < arrayList->length; index++)
    {
        if (memcmp(_arraylist_get_element_address(arrayList, index), element,arrayList->element_size )== 0)
            return index;
    }
    return -1;
}


void _arraylist_set(ArrayList *arrayList, int index, void* element)
{
    if (index < 0 || index >= arrayList->length)
    {
        return;
    }

    void* position_of_element_to_change = _arraylist_get_element_address(arrayList, index);
    memcpy(position_of_element_to_change, element, arrayList->element_size);
}


int _arraylist_count(ArrayList *arrayList, void* element)
{
    int counter = 0;
    for (int index = 0; index < arrayList->length; index++)
        if (memcmp(_arraylist_get_element_address(arrayList, index),element, arrayList->element_size) == 0)
            counter+=1;
        // se os bytes de cada posição forem iguais ao bytes do elemento passado, sendo contados até o tamanho de cada tipo forem iguais, memcmp retorna 0 
    return counter;
}



// calcula a posição na memoria do elemento a ser adicionado, endereço inicial + nº bytes até o elemento adicionado
void* _arraylist_get_element_address(ArrayList *arrayList, int index)
{
    if(index<0 || index>=arrayList->length)
    {
        return NULL;
    }
    first_element_address base = get_first_element_address(arrayList);
    return (base + index * arrayList->element_size);
}


void arraylist_clear(ArrayList *arrayList)
{
    arrayList->length=0;
    arrayList->capacity = 8;
    free(arrayList->data);
    arrayList->data = malloc(arrayList->capacity * arrayList->element_size);
}


void arraylist_free(ArrayList *arrayList)
{
    free(arrayList->data);
    free(arrayList);
}

