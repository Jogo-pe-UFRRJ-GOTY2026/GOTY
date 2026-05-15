#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Estrutura que representa um nó da LinkedList
 *
 * Cada nó armazena:
 * - um ponteiro para o valor armazenado
 * - um ponteiro para o próximo nó
 * - o índice do elemento dentro da lista
 *
 * A estrutura permite encadear elementos dinamicamente
**/
typedef struct Node{
    void* value;
    struct Node* next;
    size_t index;
} Node;
/**
 * @brief Estrutura principal da LinkedList
 *
 * Armazena o início da lista, assim como seu comprimento e o número de bytes que cada elemento ocupa
 *
 * A lista é genérica e pode armazenar qualquer tipo de dado,
 * pois os valores são manipulados através de ponteiros void*
 * e cópias de memória com memcpy.
**/
typedef struct LinkedList{
    Node* head;
    size_t length;
    size_t element_size;
} LinkedList;

/**
 * @brief Macro para inicializar uma LinkedList
 *
 * Cria uma lista capaz de armazenar elementos do tipo especificado.
 *
 * @param type tipo de dado que será armazenado na lista
 *
 * @return ponteiro para uma nova LinkedList vazia
**/
#define linkedlist_init(type) _linkedlist_init(sizeof(type))
LinkedList* _linkedlist_init(size_t element_size);



//=====================================================================================================================



/**
 * @brief Adiciona um elemento no início da lista
 *
 * @param linkedlist lista onde o elemento será inserido
 * @param value valor a ser inserido
 * @param type tipo do valor
**/
#define linkedlist_addFirst(linkedlist, value, type)                    \
    do{                                                                 \
        type temp = (value);                                            \
        _linkedlist_addFirst(linkedlist, &temp);                        \
    }while(0)
    void _linkedlist_addFirst(LinkedList *linked_list, void *value);



//=====================================================================================================================



/**
 * @brief Adiciona um elemento no final da lista
 *
 * @param linkedlist lista onde o elemento será inserido
 * @param value valor a ser inserido
 * @param type tipo do valor
**/
#define linkedlist_addLast(linkedlist, value, type) \
        do                                              \
        {                                               \
            type temp = (value);                        \
            _linkedlist_addLast(linkedlist, &temp);     \
        } while (0) 
void _linkedlist_addLast(LinkedList *linked_list, void *value);



//=====================================================================================================================



/**
 * @brief Insere um elemento em um índice específico da lista
 *
 * Os elementos posteriores são deslocados para frente.
 *
 * @param linkedlist lista onde o elemento será inserido
 * @param value valor a ser inserido
 * @param index posição desejada
 * @param type tipo do valor
**/
#define linkedlist_addAtIndex(linkedlist, value, index, type)           \
    do                                                                  \
    {                                                                   \
        type temp = (value);                                            \
        _linkedlist_addAtIndex(linkedlist, &temp, index);               \
    } while (0)
void _linkedlist_addAtIndex(LinkedList *linked_list, void *value, size_t index);



//=====================================================================================================================



/**
 * @brief Substitui o valor presente em um índice específico
 *
 * @param linkedlist lista onde o valor será modificado
 * @param value novo valor
 * @param index índice do elemento
 * @param type tipo do valor
**/
#define linkedlist_setAtIndex(linkedlist, value, index, type)           \
    do{                                                                 \
        type temp = (value);                                            \
        _linkedlist_setAtIndex(linkedlist, &temp, index);               \
    } while(0)
void _linkedlist_setAtIndex(LinkedList *linked_list, void *value, size_t index);



//=====================================================================================================================



/**
 * @brief Obtém o primeiro elemento da lista
 * Caso deseje apenas o ponteiro do elemento chame a função de mesmo nome da macro antecedidada por '_'
 *
 * @param linkedlist lista alvo
 * @param type tipo de retorno
 *
 * @return valor armazenado no primeiro nó
**/
#define linkedlist_getFirst(linkedlist, type) (*(type *) _linkedlist_getFirst(linkedlist))
void* _linkedlist_getFirst(LinkedList *linked_list) ;



//=====================================================================================================================


 
/**
 * @brief Obtém o último elemento da lista
 * Caso deseje apenas o ponteiro do elemento chame a função de mesmo nome da macro antecedidada por '_'
 * 
 * @param linkedlist lista alvo
 * @param type tipo de retorno
 *
 * @return valor armazenado no último nó
**/
#define linkedlist_getLast(linkedlist, type) (*(type *) _linkedlist_getLast(linkedlist))
void* _linkedlist_getLast(LinkedList *linked_list);



//=====================================================================================================================


/**
 * @brief Obtém o valor presente em um índice específico
 *  Caso deseje apenas o ponteiro do elemento chame a função de mesmo nome da macro antecedidada por '_'
 *
 * @param linkedlist lista alvo
 * @param index índice desejado
 * @param type tipo de retorno
 *
 * @return valor armazenado no índice
**/ 
#define linkedlist_getAtIndex(linkedlist, index, type) (*(type *)_linkedlist_getAtIndex(linkedlist, index))
void* _linkedlist_getAtIndex(LinkedList *linked_list, size_t index);



//=====================================================================================================================

/**
 * @brief Remove o primeiro elemento da lista
 *
 * Após a remoção o segundo elemento passa a ser o novo head.
**/
void linkedlist_removeFirst(LinkedList *linked_list);



//=====================================================================================================================



/**
 * @brief Remove o último elemento da lista
**/
void linkedlist_removeLast(LinkedList *linked_list);



//=====================================================================================================================



/**
 * @brief Remove o elemento presente em um índice específico
 *
 * @param linked_list lista alvo
 * @param index posição do elemento a ser removido
**/
void linkedlist_removeAtIndex(LinkedList *linked_list, size_t index);



//=====================================================================================================================



/**
 * @brief Verifica se um valor está presente na lista
 *
 * @param linkedlist lista alvo
 * @param value valor procurado
 * @param type tipo do valor
 *
 * @return true (<stdbool.h>) se o valor estiver presente
**/
#define linkedlist_contains(linkedlist,value, type)                     \
    ({                                                                  \
        type temp = (value);                                            \
        _linkedlist_contains(linkedlist, &temp);                        \
    })
bool _linkedlist_contains(LinkedList *linked_list, void *value);



//=====================================================================================================================



/**
 * @brief Remove todos os elementos da lista
 *
 * Libera todos os nós e reinicia a estrutura.
**/
void linkedlist_clear(LinkedList *linked_list);



//=====================================================================================================================



/**
 * @brief Libera completamente a memória da LinkedList
 *
 * Libera todos os nós e a própria estrutura da lista.
**/
void linkedlist_free(LinkedList *linked_list);



//=====================================================================================================================



/**
 * @brief Macro para iterar sobre todos os elementos da lista
 *
 * Executa uma função para cada valor armazenado.
 *
 * @param linked_list lista a ser iterada
 * @param func função que recebe (void*) com o valor
**/
#define linked_list_foreach(linked_list, func)                                          \
    for (Node *current = (linked_list)->head; current != NULL; current = current->next) \
    {                                                                                   \
        func(current->value);                                                           \
    }



//=====================================================================================================================



/**
 * @brief Aplica uma função a cada elemento da lista e retorna uma nova lista com os resultados
 *
 * A função passada deve receber um ponteiro para o elemento atual
 * e retornar um ponteiro para o novo valor.
 *
 * @param list lista de origem
 * @param func função que transforma cada elemento
 * @param type tipo para caso a transformação feita altera o tipo de dado dos elementos
 *
 * @return nova LinkedList contendo os valores transformados
 * É responsabilidade do usuario liberar a lista nova e adequar a função para uso da macro.
**/
#define linkedlist_map(list, func,type)                                                     \
    ({                                                                                      \
        LinkedList* new_list = _linkedlist_init(sizeof(type));                              \
                                                                                            \
        for (Node* current = list->head; current != NULL; current = current->next)          \
        {                                                                                   \
            type new_val = *(type*) func(current->value);                                   \
            _linkedlist_addLast(new_list, &new_val);                                        \
        }                                                                                   \
                                                                                            \
        new_list;                                                                           \
    })



    






#endif