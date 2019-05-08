/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

void free_expression(t_expression_tree **expression_node)
{
    if ((*expression_node)->result.value != NULL)
    {
        free((*expression_node)->result.value);
        return;
    }
    if ((*expression_node)->first != NULL)
    {
        free_expression(&(*expression_node)->first);
        free((*expression_node)->first);
    }
    if ((*expression_node)->second != NULL)
    {
        free_expression(&(*expression_node)->second);
        free((*expression_node)->second);
    }
}

t_expression_tree *parse_left_value(t_bistromathique bistromathique, int position, int size)
{
    t_expression_tree *left_expression = NULL;
    char *result = NULL;
    int i = position - 1 - size;

    if ((left_expression = malloc(sizeof(*left_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    if ((result = malloc(sizeof(*result) * (size + 1))) == NULL)
    {
        free(left_expression);
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result[size] = '\0';
    while (i++ < position - 1)
        result[size - (position - i)] = bistromathique.expr[i];
    left_expression->first = NULL;
    left_expression->second = NULL;
    left_expression->operator = 0;
    left_expression->result.value = result;
    left_expression->result.size = size;
    return left_expression;
}

t_expression_tree *parse_right_value(t_bistromathique bistromathique, int position)
{
    t_expression_tree *right_expression = NULL;
    char *result = NULL;
    int i = position + 1;
    int size = 0;

    if ((right_expression = malloc(sizeof(*right_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < bistromathique.size && !is_operator(bistromathique, bistromathique.expr[i]))
    {
        size += 1;
        i += 1;
    }
    if ((result = malloc(sizeof(*result) * (size + 1))) == NULL)
    {
        free(right_expression);
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result[size] = '\0';
    while (i-- > position + 1)
        result[i - (position + 1)] = bistromathique.expr[i];
    right_expression->first = NULL;
    right_expression->second = NULL;
    right_expression->operator = 0;
    right_expression->result.value = result;
    right_expression->result.size = size;
    return right_expression;
}

int update_root_expression(t_bistromathique bistromathique, t_expression_tree **root,
                           t_expression_tree *new_expression_node)
{
    if ((*root)->first == NULL || (*root)->second == NULL)
    {
        free_expression(root);
        *root = new_expression_node;
        return 0;
    }
    if (is_priority_operator(bistromathique, new_expression_node->operator))
    {
        new_expression_node->first = (*root)->second;
        (*root)->second = new_expression_node;
    }
    else
    {
        free_expression(&new_expression_node->first);
        new_expression_node->first = *root;
        *root = new_expression_node;
    }
    return 0;
}

t_expression_tree *create_expression(t_bistromathique bistromathique, int position, int previous_element_size)
{
    t_expression_tree *new_expression_node = NULL;

    if ((new_expression_node = malloc(sizeof(*new_expression_node))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    new_expression_node->first = NULL;
    new_expression_node->second = NULL;
    if ((new_expression_node->first = parse_left_value(bistromathique, position, previous_element_size)) == NULL)
    {
        free(new_expression_node);
        return NULL;
    };
    if ((new_expression_node->second = parse_right_value(bistromathique, position)) == NULL)
    {
        free(new_expression_node->first);
        free(new_expression_node);
        return NULL;
    };
    new_expression_node->operator = bistromathique.expr[position];
    new_expression_node->result.value = NULL;
    new_expression_node->result.size = 0;
    return new_expression_node;
}