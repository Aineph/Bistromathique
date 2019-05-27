/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

void empty_operation_list(t_operation_list *operation_list)
{
    t_operation_list *operation_tmp = operation_list;
    t_operation_list *operation_save;

    while (operation_tmp != NULL)
    {
        operation_save = operation_tmp->next;
        free(operation_tmp);
        operation_tmp = operation_save;
    }
}

int add_operation_in_list(t_operation_list **operation_list, char operator, t_operation operation)
{
    t_operation_list *new_operation = NULL;

    if ((new_operation = malloc(sizeof(*new_operation))) == NULL)
        return -1;
    new_operation->operator = operator;
    new_operation->operation = operation;
    new_operation->next = *operation_list;
    *operation_list = new_operation;
    return 0;
}

t_operation_list *init_operation_list(t_bistromathique bistromathique)
{
    t_operation_list *operation_list = NULL;

    if (add_operation_in_list(&operation_list, bistromathique.ops[OP_PLUS_IDX], &infinite_add) == -1)
        return NULL;
    if (add_operation_in_list(&operation_list, bistromathique.ops[OP_SUB_IDX], &infinite_sub) == -1)
    {
        empty_operation_list(operation_list);
        return NULL;
    }
    if (add_operation_in_list(&operation_list, bistromathique.ops[OP_MULT_IDX], &infinite_mul) == -1)
    {
        empty_operation_list(operation_list);
        return NULL;
    }
    if (add_operation_in_list(&operation_list, bistromathique.ops[OP_DIV_IDX], &infinite_div) == -1)
    {
        empty_operation_list(operation_list);
        return NULL;
    }
    if (add_operation_in_list(&operation_list, bistromathique.ops[OP_MOD_IDX], &infinite_modulo) == -1)
    {
        empty_operation_list(operation_list);
        return NULL;
    }
    return operation_list;
}