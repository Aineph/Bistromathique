/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Removes the top element in the stack of expressions.
 * @param expression_stack: The stack of expressions.
 * @return: A negative value if an error occurs. Zero if the removal completes successfully.
 */
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

/**
 * Frees the whole stack of expressions. Also frees the expression trees that are stored inside.
 * @param expression_stack: The stack of expressions.
 * @return: A negative value if an error occurs. Zero if the deletion completes succesfully.
 */
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

/**
 * Merges the expression tree of the top element in the stack with the expression tree of the element below.
 * @param expression_stack: The stack of expressions.
 * @return: A negative value if an error occurs. Zero if the merge completes successfully.
 */
int merge_expressions(t_expression_stack **expression_stack)
{
    t_expression_stack *super_expression = (*expression_stack)->next;
    t_expression_tree *expression_tmp = NULL;

    if (super_expression == NULL)
    {
        my_putstr(SYNTAXE_ERROR_MSG);
        return -1;
    }
    if (super_expression->expression_root->first == NULL || super_expression->expression_root->second == NULL)
    {
        free_expression(&super_expression->expression_root);
        super_expression->expression_root = (*expression_stack)->expression_root;
    }
    else
    {
        expression_tmp = super_expression->expression_root;

        while (expression_tmp->second != NULL && expression_tmp->second->second != NULL)
            expression_tmp = expression_tmp->second;
        free_expression(&expression_tmp->second);
        expression_tmp->second = (*expression_stack)->expression_root;
    }
    pop_expression_from_stack(expression_stack);
    return 0;
}

/**
 * Adds an expression tree into an expression_stack.
 * @param expression_stack: The stack of expressions.
 * @param expression_root: The root of the expression tree to push in the stack.
 * @return: A negative value if an error occurs. Zero if the insertion completes successfully.
 */
static int add_expression_to_stack(t_expression_stack **expression_stack, t_expression_tree *expression_root)
{
    t_expression_stack *new_element;

    if ((new_element = malloc(sizeof(*new_element))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    new_element->expression_root = expression_root;
    new_element->next = *expression_stack;
    *expression_stack = new_element;
    return 0;
}

/**
 * Initializes an expression tree and adds it to an expression stack.
 * @param expression_stack: The stack of expressions.
 * @param expression_level: The current parenthesis level in the expression.
 * @param position: The current position in the expression.
 * @param expression_level: The current parenthesis level in the expression.
 * @return: A negative value if an error occurs. Zero if the creation completes successfully.
 */
int create_sub_expression(t_bistromathique bistromathique, t_expression_stack **expression_stack, int position,
                          int expression_level)
{
    t_expression_tree *expression_root = NULL;

    if ((expression_root = expression_has_no_operator(bistromathique, position, expression_level)) == NULL)
    {
        if ((expression_root = malloc(sizeof(*expression_root))) == NULL)
        {
            my_putstr(MALLOC_ERROR);
            return -1;
        }
        expression_root->first = NULL;
        expression_root->second = NULL;
        expression_root->operator = 0;
        expression_root->level = expression_level;
        expression_root->result = NULL;
    }
    if (add_expression_to_stack(expression_stack, expression_root) == -1)
    {
        free_number(expression_root->result);
        free(expression_root);
        return -1;
    }
    return 0;
}
