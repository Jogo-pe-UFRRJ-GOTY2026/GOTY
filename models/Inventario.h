#ifndef INVENTARIO_H
#define INVENTARIO_H
#include "Items.h"
#include "../data_structures/hashtable.h"

typedef struct Inventario{
    hash_table* armadura;
    Arma* arma;
    hash_table* consumiveis;
} Inventario;

#endif