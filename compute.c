/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

char *compute(t_bistromathique bistromathique, t_operation_list *operation_list, t_expression_tree *expression_node)
{
    t_operation_list *operation_tmp = operation_list;
    char *nb_a = NULL;
    char *nb_b = NULL;

    if (expression_node->result != NULL)
        return expression_node->result;
    if (expression_node->first != NULL)
        nb_a = compute(bistromathique, operation_list, expression_node->first);
    if (expression_node->second != NULL)
        nb_b = compute(bistromathique, operation_list, expression_node->second);
    while (operation_tmp != NULL)
    {
        if (operation_tmp->operator == expression_node->operator)
            return operation_tmp->operation(bistromathique, nb_a, nb_b);
        operation_tmp = operation_tmp->next;
    }
    return NULL;
}
