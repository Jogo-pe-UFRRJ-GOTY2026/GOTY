#include "hashset.h"
#include <stdlib.h>
#include <string.h>

// Aproveitei pra fazer isso pois é o leetcode 705

#define initial_capacity 50

                
// eu fui ver como fazer hash em void* e descobri que char pode ser signed dependendo do compilador, wtf????    


// Ta, ainda vou ter o problema do size, pq eu nao preciso o size do ponteiro, mas do objeto que estara nele, isso sera um problema, a nao ser que eu deixe pro usuario da biblioteca
size_t djb2(void * val, size_t size)  
{
    unsigned char *p = (unsigned char*) val;
    const int DJB2_INITIAL = 5381;
    const int DJB2_MULTIPLIER = 33;

    size_t hash_value = DJB2_INITIAL;
    for (size_t i = 0; i<size; i++)
    {
        hash_value = hash_value * DJB2_MULTIPLIER + p[i];
    }
    return hash_value;
}

size_t h1(void *val, size_t capacity)
{
    return djb2(val, sizeof(val)) % capacity;
}
size_t h2(void *val, size_t capacity)
{
    return 1+ djb2(val, sizeof(val)) % (capacity-1);
}







HashSet* init_hashset()
{
    HashSet* self = malloc(sizeof(HashSet));
    if (self == NULL)exit(1);

    self->capacity = initial_capacity;


    self->set = malloc(initial_capacity*sizeof(set_obj));
    if(self->set==NULL) exit(1);

    for (size_t i = 0; i < initial_capacity; i++)
    {
        self->set[i] = (set_obj *)malloc(sizeof(set_obj));
        self->set[i]->value=NULL;
        self->set[i]->element_size=0;
    }
}



void hashset_add(HashSet* self, void* value, size_t element_size)
{
    size_t index_base = h1(value, self->capacity);
    size_t index_step = h2(value, self->capacity);

    for (int i = 0; i < self->capacity; i++)
    {
        size_t index = (index_base + index_step * i) % self->capacity;
        set_obj* obj = self->set[index];
        if(obj==NULL) continue;
        if(obj->value==NULL)
        {
            obj->value = malloc(element_size);
            obj->element_size = element_size;
            memcpy(obj->value, value, element_size);
        }
        else continue; // busca o proximo espaço nao ocupado
    }
}

void hashset_remove(HashSet *self, void *value, size_t element_size)
{
    size_t index_base = h1(value, self->capacity);
    size_t index_step = h2(value, self->capacity);

    for (int i = 0; i < self->capacity; i++)
    {
        size_t index = (index_base + index_step * i) % self->capacity;
        set_obj *obj = self->set[index];
        if (obj == NULL)
            continue;
        if (memcmp(obj->value, value, element_size )==0)
        {
            free(obj->value);
            obj->element_size = 0;
        }
        else
            continue; // busca o proximo espaço nao ocupado
    }
}

void hashset_contains(HashSet *self, void *value, size_t element_size)
{
    size_t index_base = h1(value, self->capacity);
    size_t index_step = h2(value, self->capacity);

    for (int i = 0; i < self->capacity; i++)
    {
        size_t index = (index_base + index_step * i) % self->capacity;
        set_obj *obj = self->set[index];
        if (obj == NULL)
            continue;

        if (memcmp(obj->value, value, element_size)==0)
        {
            free(obj->value);
            obj->element_size = 0;
        }
        else
            continue; // busca o proximo espaço nao ocupado
    }
}

void hashset_clear(HashSet *self)
{
    for (int i = 0; i < self->capacity; i++)
    {
        free(self->set[i]->value);
        self->set[i]->element_size=0;
    }
}
void hashset_free(HashSet *self)
{
    if(self==NULL) exit(1);
    for(int i=0;i<self->capacity;i++)
    {
        free(self->set[i]->value);
        free(self->set[i]);
    }
    free(self);
}