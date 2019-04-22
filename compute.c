/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

char *compute(t_bistromathique bistromathique, t_operation *operations, t_expression *expression)
{
    t_operation *operation = operations;
    char *nb_a = NULL;
    char *nb_b = NULL;

    if (expression->result != NULL)
        return expression->result;
    if (expression->first != NULL)
        nb_a = compute(bistromathique, operations, expression->first);
    if (expression->second != NULL)
        nb_b = compute(bistromathique, operations, expression->second);
    while (operation != NULL)
    {
        if (operation->operator == expression->operator)
            return operation->operation(bistromathique, nb_a, nb_b);
        operation = operation->next;
    }
    return NULL;
}
