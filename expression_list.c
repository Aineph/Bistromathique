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
    {
        my_putstr(SYNTAXE_ERROR_MSG);
        return -1;
    }
    *list = (*list)->next;
    free(save);
    return 0;
}

int empty_expression_list(t_expression_list **list)
{
    while (*list != NULL)
    {
        free_expression(&(*list)->expression_root);
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
    if (super_expression->expression_root->first == NULL || super_expression->expression_root->second == NULL)
        super_expression->expression_root = (*list)->expression_root;
    else
        super_expression->expression_root->second = (*list)->expression_root;
    pop_expression_from_list(list);
    return 0;
}

int add_expression_to_list(t_expression_list **list, t_expression_tree *expression_root)
{
    t_expression_list *new_element;

    if ((new_element = malloc(sizeof(*new_element))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    new_element->expression_root = expression_root;
    new_element->next = *list;
    *list = new_element;
    return 0;
}

int init_expression_in_list(t_expression_list **list)
{
    t_expression_tree *expression_root = NULL;

    if ((expression_root = malloc(sizeof(*expression_root))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    expression_root->first = NULL;
    expression_root->second = NULL;
    expression_root->operator = 0;
    if ((expression_root->result = create_number()) == NULL)
    {
        free(expression_root);
        return -1;
    }
    expression_root->result->value = NULL;
    expression_root->result->size = 0;
    if (add_expression_to_list(list, expression_root) == -1)
    {
        free(expression_root);
        empty_expression_list(list);
        return -1;
    }
    return 0;
}