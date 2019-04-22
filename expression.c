/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

void free_expression(t_expression **expression)
{
    if ((*expression)->result != NULL)
    {
        free((*expression)->result);
        return;
    }
    if ((*expression)->first != NULL)
    {
        free_expression(&(*expression)->first);
        free((*expression)->first);
    }
    if ((*expression)->second != NULL)
    {
        free_expression(&(*expression)->second);
        free((*expression)->second);
    }
}

t_expression *parse_left_value(t_bistromathique bistromathique, int position)
{
    t_expression *left_expression = NULL;
    char *result = NULL;
    int i = position - 1;

    if ((left_expression = malloc(sizeof(*left_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i >= 0 && !is_operator(bistromathique.ops, bistromathique.expr[i]))
    {
        result = concat_before(result, bistromathique.expr[i]);
        i -= 1;
    }
    left_expression->first = NULL;
    left_expression->second = NULL;
    left_expression->operator = 0;
    left_expression->result = result;
    return left_expression;
}

t_expression *parse_right_value(t_bistromathique bistromathique, int position)
{
    t_expression *left_expression = NULL;
    char *result = NULL;
    int i = position + 1;

    if ((left_expression = malloc(sizeof(*left_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < bistromathique.size && !is_operator(bistromathique.ops, bistromathique.expr[i]))
    {
        result = concat_after(result, bistromathique.expr[i]);
        i += 1;
    }
    left_expression->first = NULL;
    left_expression->second = NULL;
    left_expression->operator = 0;
    left_expression->result = result;
    return left_expression;
}

int update_root_expression(t_bistromathique bistromathique, t_expression **root, t_expression *new_expression)
{
    if ((*root)->first == NULL || (*root)->second == NULL)
    {
        *root = new_expression;
        return 0;
    }
    if (is_priority_operator(bistromathique.ops, new_expression->operator))
    {
        new_expression->first = (*root)->second;
        (*root)->second = new_expression;
    }
    else
    {
        free_expression(&new_expression->first);
        new_expression->first = *root;
        *root = new_expression;
    }
    return 0;
}

t_expression *create_expression(t_bistromathique bistromathique, int position)
{
    t_expression *new = NULL;

    if ((new = malloc(sizeof(*new))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    new->first = NULL;
    new->second = NULL;
    if ((new->first = parse_left_value(bistromathique, position)) == NULL)
    {
        free(new);
        return NULL;
    };
    if ((new->second = parse_right_value(bistromathique, position)) == NULL)
    {
        free(new->first);
        free(new);
        return NULL;
    };
    new->operator = bistromathique.expr[position];
    new->result = NULL;
    return new;
}