/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int pop_expression_from_list(t_expression_list **list)
{
    t_expression_list *save = *list;

    if (*list == NULL)
        return -1;
    *list = (*list)->next;
    free(save);
    return 0;
}

int empty_expression_list(t_expression_list **list)
{
    while (*list != NULL)
    {
        free_expression(&(*list)->expression);
        if (pop_expression_from_list(list) == -1)
            return -1;
    }
    return 0;
}

int merge_expressions(t_expression_list **list)
{
    t_expression_list *super_expression = (*list)->next;

    if (super_expression == NULL)
        return -1;
    if (super_expression->expression->first == NULL || super_expression->expression->second == NULL)
        super_expression->expression = (*list)->expression;
    else
        super_expression->expression->second = (*list)->expression;
    pop_expression_from_list(list);
    return 0;
}

int add_expression_to_list(t_expression_list **list, t_expression *expression)
{
    t_expression_list *new_element;

    if ((new_element = malloc(sizeof(*new_element))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    new_element->expression = expression;
    new_element->next = *list;
    *list = new_element;
    return 0;
}

int init_expression_in_list(t_expression_list **list)
{
    t_expression *expression = NULL;

    if ((expression = malloc(sizeof(*expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    expression->first = NULL;
    expression->second = NULL;
    expression->operator = 0;
    expression->result = NULL;
    if (add_expression_to_list(list, expression) == -1)
    {
        free(expression);
        empty_expression_list(list);
        return -1;
    }
    return 0;
}