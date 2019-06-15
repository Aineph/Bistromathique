/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the whole map of operation.
 */
void empty_operation_map(t_operation_map *operation_map)
{
    t_operation_map *operation_tmp = operation_map;
    t_operation_map *operation_save;

    while (operation_tmp != NULL)
    {
        operation_save = operation_tmp->next;
        free(operation_tmp);
        operation_tmp = operation_save;
    }
}

/**
 * Adds an operation to the operation map.
 * @param operation_map: A pointer to the operation map.
 * @param operator: The operator character.
 * @param operation: The operation function to associate to the operator in the map.
 * @return: A negative value if an error occurs. Zero if the insertion completes successfully.
 */
static int add_operation_in_map(t_operation_map **operation_map, char operator, t_operation operation)
{
    t_operation_map *new_operation = NULL;

    if ((new_operation = malloc(sizeof(*new_operation))) == NULL)
        return -1;
    new_operation->operator = operator;
    new_operation->operation = operation;
    new_operation->next = *operation_map;
    *operation_map = new_operation;
    return 0;
}

/**
 * Initializes the operation map.
 * @param bistromathique: The bistromathique structure.
 * @return: The map of operations associating the character of the operation to its function.
 */
t_operation_map *init_operation_map(t_bistromathique bistromathique)
{
    t_operation_map *operation_map = NULL;

    if (add_operation_in_map(&operation_map, bistromathique.ops[OP_PLUS_IDX], &infinite_add) == -1)
        return NULL;
    if (add_operation_in_map(&operation_map, bistromathique.ops[OP_SUB_IDX], &infinite_sub) == -1)
    {
        empty_operation_map(operation_map);
        return NULL;
    }
    if (add_operation_in_map(&operation_map, bistromathique.ops[OP_MULT_IDX], &infinite_mul) == -1)
    {
        empty_operation_map(operation_map);
        return NULL;
    }
    if (add_operation_in_map(&operation_map, bistromathique.ops[OP_DIV_IDX], &infinite_div) == -1)
    {
        empty_operation_map(operation_map);
        return NULL;
    }
    if (add_operation_in_map(&operation_map, bistromathique.ops[OP_MOD_IDX], &infinite_mod) == -1)
    {
        empty_operation_map(operation_map);
        return NULL;
    }
    return operation_map;
}
