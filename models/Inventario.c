#include "Inventario.h"
#include "../data_structures/hashtable.h"
#include "../utils/utils.h"
#include "Items.h"
#include <stdlib.h>
#include <string.h>


Inventario *init_inventario()
{
    Inventario *inventario = malloc(sizeof(Inventario));
    if (inventario == NULL)
        exit_with_error(Exception_AllocationError);

    inventario->armadura = hash_table_init();
    if (inventario->armadura == NULL)
        exit_with_error(Exception_AllocationError);

    hash_table_update(inventario->armadura, "peitoral", 1, sizeof(int));
    hash_table_update(inventario->armadura, "luva", 2, sizeof(int));
    hash_table_update(inventario->armadura, "bota", 3, sizeof(int));

    inventario->arma = criar_arma("Espada inicial", "Espada inicial fraca",4, CORTE);

    inventario->consumiveis = hash_table_init();
    if (inventario->consumiveis == NULL)
        exit_with_error(Exception_AllocationError);

    return inventario;
}