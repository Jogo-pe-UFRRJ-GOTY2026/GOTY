#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define INITIAL_TABLE_SIZE 101

typedef enum Status
{
    EMPTY,
    OCCUPIED,
    DELETED
} Status;

typedef struct Item
{
    char *key;
    void *value;
    Status status;
} Item;

typedef struct _hash_table{
    Item** hash_table;
    size_t capacity;
} hash_table;

hash_table* hash_table_init();


bool hash_table_update(hash_table *hashtable, const char* key, void* value, size_t element_size);
void* hash_table_get(hash_table *hashtable, const char *key);
bool hash_table_delete(hash_table *ht, const char *key);

void hash_table_clear(hash_table *hashtable);
void hash_table_free(hash_table* hashtable);

#endif