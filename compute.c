/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *
compute(t_bistromathique bistromathique, t_operation_list *operation_list, t_expression_tree **expression_node)
{
    t_number *nb_a = NULL;
    t_number *nb_b = NULL;

    if ((*expression_node)->result->value != NULL)
        return (*expression_node)->result;
    if ((*expression_node)->first != NULL)
        nb_a = compute(bistromathique, operation_list, &(*expression_node)->first);
    if ((*expression_node)->second != NULL)
        nb_b = compute(bistromathique, operation_list, &(*expression_node)->second);
    while (operation_list != NULL)
    {
        if (operation_list->operator == (*expression_node)->operator)
        {
            (*expression_node)->result = operation_list->operation(bistromathique, nb_a, nb_b);
            free_expression(&(*expression_node)->first);
            free_expression(&(*expression_node)->second);
            (*expression_node)->first = NULL;
            (*expression_node)->second = NULL;
            return (*expression_node)->result;
        }
        operation_list = operation_list->next;
    }
    return NULL;
}
