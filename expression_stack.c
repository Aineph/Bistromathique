/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int pop_expression_from_stack(t_expression_stack **expression_stack)
{
    t_expression_stack *save = *expression_stack;

    if (*expression_stack == NULL)
    {
        my_putstr(SYNTAXE_ERROR_MSG);
        return -1;
    }
    *expression_stack = (*expression_stack)->next;
    free(save);
    return 0;
}

int empty_expression_stack(t_expression_stack **expression_stack)
{
    while (*expression_stack != NULL)
    {
        free_expression(&(*expression_stack)->expression_root);
        if (pop_expression_from_stack(expression_stack) == -1)
            return -1;
    }
    return 0;
}

int merge_expressions(t_expression_stack **expression_stack)
{
    t_expression_stack *super_expression = (*expression_stack)->next;
    t_expression_tree *expression_root;

    if (super_expression == NULL)
        return -1;
    if (super_expression->expression_root->first == NULL || super_expression->expression_root->second == NULL)
        super_expression->expression_root = (*expression_stack)->expression_root;
    else
    {
        expression_root = super_expression->expression_root;

        while (expression_root->second != NULL && expression_root->second->second != NULL)
            expression_root = expression_root->second;
        free_expression(&expression_root->second);
        expression_root->second = (*expression_stack)->expression_root;
    }
    pop_expression_from_stack(expression_stack);
    return 0;
}

int add_expression_to_stack(t_expression_stack **list, t_expression_tree *expression_root)
{
    t_expression_stack *new_element;

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

int create_sub_expression(t_expression_stack **expression_stack, int level)
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
    expression_root->level = level;
    if ((expression_root->result = create_number()) == NULL)
    {
        free(expression_root);
        return -1;
    }
    expression_root->result->value = NULL;
    expression_root->result->size = 0;
    if (add_expression_to_stack(expression_stack, expression_root) == -1)
    {
        free(expression_root);
        empty_expression_stack(expression_stack);
        return -1;
    }
    return 0;
}