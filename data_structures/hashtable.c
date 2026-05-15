#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>





bool str_equals(const char* key1, const char* key2)
{
    return strcmp(key1, key2)==0;
}
bool isNULL(const char* x) // Escolha por legibilidade
{
    return x==NULL;
}

uint64_t djb2(const char *key)
{
    const int DJB2_INITIAL = 5381;
    const int DJB2_MULTIPLIER = 33;

    uint64_t hash_value = DJB2_INITIAL;
    for (size_t i = 0; key[i] !='\0'; i++)
    {
        hash_value = hash_value * DJB2_MULTIPLIER + key[i];
    }
    return hash_value;
}

hash_table* hash_table_init()
{
    hash_table* ht = malloc(sizeof(hash_table));
    if(ht == NULL)
    {
        exit(1);
    }

    ht->capacity = INITIAL_TABLE_SIZE;
    ht->hash_table = malloc(sizeof(Item*) * ht->capacity);

    for(size_t i = 0;i<ht->capacity;i++)
    {
        ht->hash_table[i] = (Item*) malloc(sizeof(Item));
        ht->hash_table[i]->key = NULL;
        ht->hash_table[i]->value = NULL;
        ht->hash_table[i]->status = EMPTY;
    }
    return ht;
}


uint64_t linear_probing(uint64_t hashindex, size_t i, size_t capacity)
{
    return (hashindex+i) % capacity;
}

uint64_t quadratic_probing(uint64_t hashindex, size_t i, size_t capacity)
{
    return (hashindex + i*i) % capacity;
}


uint64_t h1(const char*key, size_t capacity)
{
    return djb2(key) % capacity; 
}
uint64_t h2(const char*key, size_t capacity)
{
    return 1 + (djb2(key) % (capacity-1));
}


bool hash_table_update(hash_table *ht,const char *key, void* val, size_t element_size)
{
    if( isNULL(key) || isNULL(val) || str_equals(key, "") ) 
        return false;

    //uint64_t hash_index = hash(key, ht->capacity); //para linear e quadratic
    uint64_t index_base = h1(key, ht->capacity);
    uint64_t index_step = h2(key, ht->capacity);

    for(size_t i = 0; i < ht->capacity; i++)
    {
        // uint64_t index = linear_probing(hash_index,i,ht->capacity);
        // uint64_t index = quadratic_probing(hash_index,i,ht->capacity);

        uint64_t index = (index_base + index_step*i) % ht->capacity; 
        //o modulo garante que nunca caira para um valor fora de capacity-1, o index step é um numero gerado por hash determinando o pulo entre cada
        // checagem minimizando o encontro de clusters de dados de tabelas comparado a linear probing, assim, se o hash do valor determinar pro indice 3, e o hash pro step for 5
        // vamo buscar em 3, 8, 13, ... até dar a volta no hashmap

        Item* item = ht->hash_table[index];
        if(isNULL(item)) continue;
        // ================ Primeiro caso, entrada vazia
        if(item->status == EMPTY) 
        {
            item->key = strdup(key);

            item->value = malloc(element_size);
            memcpy(item->value, val, element_size);
            item->status = OCCUPIED;

            return true;
        }
        // ================ Segundo caso, entrada preenchida, mesma chave, atualiza o valor(por isso o nome da funcao é update e nao add, python vibes)
        // ================ Se tiver ocupado mas nao ter a mesma chave vamo continuar buscando ne
        else if (item->status == OCCUPIED && str_equals(item->key, key))
        {
            memcpy(item->value, val, element_size);
            return true;
        }

    }
    return false; // não achou espaço para por
}

void* hash_table_get(hash_table *ht, const char *key)
{
    if (isNULL(key) || str_equals(key, ""))
        return NULL;

    // uint64_t hash_index = hash(key, ht->capacity); //para linear e quadratic
    uint64_t index_base = h1(key, ht->capacity);
    uint64_t index_step = h2(key, ht->capacity);
    for(size_t i = 0; i < ht->capacity; i++)
    {

        // uint64_t index = linear_probing(hash_index,i,ht->capacity);
        // uint64_t index = quadratic_probing(hash_index,i,ht->capacity);
        uint64_t index = (index_base + index_step * i) % ht->capacity;
        Item *item = ht->hash_table[index];
        if(isNULL(item)) continue;;
        if (item->status == EMPTY)
        {
            return NULL;
        }
        if (item->status == OCCUPIED && str_equals(item->key, key))
            return item->value;
    }
    return NULL;
}

bool hash_table_delete(hash_table *ht, const char *key)
{
    if (isNULL(key) || str_equals(key, ""))
        return false;
    // uint64_t hash_index = hash(key, ht->capacity); //para linear e quadratic
    uint64_t index_base = h1(key, ht->capacity);
    uint64_t index_step = h2(key, ht->capacity);

    for (size_t i = 0; i < ht->capacity; i++)
    {
        // uint64_t index = linear_probing(hash_index,i,ht->capacity);
        // uint64_t index = quadratic_probing(hash_index,i,ht->capacity);
        uint64_t index = (index_base + index_step * i) % ht->capacity;

        Item *item = ht->hash_table[index];
        if(isNULL(item)) continue;

        if (item->status == EMPTY)
            return false;

        if (item->status == OCCUPIED && str_equals(item->key, key))
        {
            free(item->key);
            free(item->value);

            item->key = NULL;
            item->value=NULL;
            item->status=DELETED;
            return true;
        }
    }
    return false;
}







void hash_table_free(hash_table* ht)
{
    if(ht==NULL) return ;

    for(size_t i = 0; i < ht->capacity; i++)
    {
        free(ht->hash_table[i]->key);
        free(ht->hash_table[i]->value);
        free(ht->hash_table[i]);
    }
    free(ht->hash_table);
    free(ht);

}

void hash_table_clear(hash_table *ht)
{
    if (ht == NULL)
        return;

    for(size_t i = 0; i < ht->capacity; i++)
    {
        free(ht->hash_table[i]->value);
        free(ht->hash_table[i]);
    }

    ht->capacity = INITIAL_TABLE_SIZE;
    for(size_t i = 0; i < ht->capacity; i++)
    {
        ht->hash_table[i] = (Item *)malloc(sizeof(Item));
    }
}