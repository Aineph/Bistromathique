/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

void free_operations(t_operation *operations)
{
    t_operation *tmp = operations;
    t_operation *save;

    while (tmp != NULL)
    {
        save = tmp->next;
        free(tmp);
        tmp = save;
    }
}

int add_operation(t_operation **operations, char operator, char *(*function)(t_bistromathique, char *, char *))
{
    t_operation *operation = NULL;
    t_operation *tmp;

    if ((operation = malloc(sizeof(*operation))) == NULL)
        return -1;
    operation->operator = operator;
    operation->operation = function;
    operation->next = NULL;
    if (*operations == NULL)
        *operations = operation;
    else
    {
        tmp = *operations;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = operation;
    }
    return 0;
}

t_operation *init_operations(t_bistromathique bistromathique)
{
    t_operation *operations = NULL;

    if (add_operation(&operations, bistromathique.ops[OP_PLUS_IDX], &infinite_add) == -1)
        return NULL;
    if (add_operation(&operations, bistromathique.ops[OP_SUB_IDX], &infinite_add) == -1)
    {
        free_operations(operations);
        return NULL;
    }
    if (add_operation(&operations, bistromathique.ops[OP_MULT_IDX], &infinite_add) == -1)
    {
        free_operations(operations);
        return NULL;
    }
    if (add_operation(&operations, bistromathique.ops[OP_DIV_IDX], &infinite_add) == -1)
    {
        free_operations(operations);
        return NULL;
    }
    if (add_operation(&operations, bistromathique.ops[OP_MOD_IDX], &infinite_add) == -1)
    {
        free_operations(operations);
        return NULL;
    }
    return operations;
}