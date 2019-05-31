/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Recursively navigate through the expression tree and computes the result of the children nodes.
 * @param bistromathique: The bistromathique structure.
 * @param operation_map: The map of operations associating the character of the operation to its function.
 * @param expression_node: The current node to compute in the expression tree.
 * @return: The operation result of the two children nodes.
 */
t_number *compute(t_bistromathique bistromathique, t_operation_map *operation_map, t_expression_tree **expression_node)
{
    t_number *nb_a = NULL;
    t_number *nb_b = NULL;

    if ((*expression_node)->result != NULL && (*expression_node)->result->value != NULL)
        return (*expression_node)->result;
    if ((*expression_node)->first != NULL)
        if ((nb_a = compute(bistromathique, operation_map, &(*expression_node)->first)) == NULL)
            return NULL;
    if ((*expression_node)->second != NULL)
        if ((nb_b = compute(bistromathique, operation_map, &(*expression_node)->second)) == NULL)
            return NULL;
    while (operation_map != NULL)
    {
        if (operation_map->operator == (*expression_node)->operator)
        {
            (*expression_node)->result = operation_map->operation(bistromathique, nb_a, nb_b);
            free_expression(&(*expression_node)->first);
            free_expression(&(*expression_node)->second);
            (*expression_node)->first = NULL;
            (*expression_node)->second = NULL;
            return (*expression_node)->result;
        }
        operation_map = operation_map->next;
    }
    return NULL;
}
