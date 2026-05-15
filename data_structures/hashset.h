#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct set_obj {
    void* value;
    size_t element_size;
} set_obj;

typedef struct HashSet
{
    set_obj** set;
    size_t capacity;
} HashSet;

#endif