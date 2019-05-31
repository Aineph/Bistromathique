/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the content of an expression tree.
 * @param expression_node: A pointer on the root of the expression tree to free.
 */
void free_expression(t_expression_tree **expression_node)
{
    if ((*expression_node)->result->value != NULL)
    {
        free_number((*expression_node)->result);
        return;
    }
    if ((*expression_node)->first != NULL)
    {
        free_expression(&(*expression_node)->first);
        free((*expression_node)->first);
        (*expression_node)->first = NULL;
    }
    if ((*expression_node)->second != NULL)
    {
        free_expression(&(*expression_node)->second);
        free((*expression_node)->second);
        (*expression_node)->second = NULL;
    }
}

/**
 * Creates an expression containing the number on the left of the operator.
 * @param bistromathique: The bistromathique structure.
 * @param position: The position of an operator in the arithmetic expression.
 * @return: The expression containing the left value of the expression.
 */
t_expression_tree *parse_left_value(t_bistromathique bistromathique, int position)
{
    t_expression_tree *left_expression = NULL;
    int i = position - 1;
    int size = 0;

    if ((left_expression = malloc(sizeof(*left_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i >= 0 && !is_operator(bistromathique, bistromathique.expr[i]))
        i -= 1;
    size = (position - 1) - i;
    if ((left_expression->result = create_number()) == NULL)
    {
        free(left_expression);
        return NULL;
    }
    if (assign_value_to_number(left_expression->result, &bistromathique.expr[i + 1], size, SIGN_POS) == -1)
    {
        free_number(left_expression->result);
        free(left_expression);
        return NULL;
    }
    left_expression->first = NULL;
    left_expression->second = NULL;
    left_expression->operator = 0;
    return left_expression;
}

/**
 * Creates an expression containing the number on the right of the operator.
 * @param bistromathique: The bistromathique structure.
 * @param position: The position of an operator in the arithmetic expression.
 * @return: The expression containing the right value of the expression.
 */
t_expression_tree *parse_right_value(t_bistromathique bistromathique, int position)
{
    t_expression_tree *right_expression = NULL;
    int i = position + 1;
    int size = 0;

    if ((right_expression = malloc(sizeof(*right_expression))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < bistromathique.size && !is_operator(bistromathique, bistromathique.expr[i]))
        i += 1;
    size = i - (position + 1);
    if ((right_expression->result = create_number()) == NULL)
    {
        free(right_expression);
        return NULL;
    }
    if (assign_value_to_number(right_expression->result, &bistromathique.expr[position + 1], size, SIGN_POS) == -1)
    {
        free_number(right_expression->result);
        free(right_expression);
        return NULL;
    }
    right_expression->first = NULL;
    right_expression->second = NULL;
    right_expression->operator = 0;
    return right_expression;
}

/**
 * Inserts a newly created node in the tree by taking care of priority operators.
 * @param bistromathique: The bistromathique structure.
 * @param root: The root of the expression tree.
 * @param new_expression_node: The new expression node to insert in the tree.
 */
void update_root_expression(t_bistromathique bistromathique, t_expression_tree **root,
                            t_expression_tree *new_expression_node)
{
    if ((*root)->first == NULL || (*root)->second == NULL)
    {
        free_expression(root);
        *root = new_expression_node;
    }
    else if (is_priority_operator(bistromathique, new_expression_node->operator) &&
             new_expression_node->level >= (*root)->level)
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
}

/**
 * Creates an expression node according to the index of an operator within the arithmetic expression.
 * @param bistromathique: The bistromathique structure.
 * @param position: The position of an operator in the arithmetic expression.
 * @param expression_level: The current parenthesis level in the expression.
 * @return: The newly created node.
 */
t_expression_tree *create_expression(t_bistromathique bistromathique, int position, int expression_level)
{
    t_expression_tree *new_expression_node = NULL;

    if ((new_expression_node = malloc(sizeof(*new_expression_node))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    new_expression_node->first = NULL;
    new_expression_node->second = NULL;
    if ((new_expression_node->first = parse_left_value(bistromathique, position)) == NULL)
    {
        free(new_expression_node);
        return NULL;
    }
    if ((new_expression_node->second = parse_right_value(bistromathique, position)) == NULL)
    {
        free(new_expression_node->first);
        free(new_expression_node);
        return NULL;
    }
    new_expression_node->operator = bistromathique.expr[position];
    new_expression_node->level = expression_level;
    if ((new_expression_node->result = create_number()) == NULL)
        return NULL;
    return new_expression_node;
}