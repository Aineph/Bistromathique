/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include <stdio.h>
#include "bistromathique.h"

/*
 * To remove after ending development.
 */
void print_expr(t_expression_tree *expression_node)
{
    if (expression_node->result->value != NULL)
    {
        printf("Value: %s\n", expression_node->result->value);
        return;
    }
    if (expression_node->first != NULL)
        print_expr(expression_node->first);
    printf("%c\n", expression_node->operator);
    if (expression_node->second != NULL)
        print_expr(expression_node->second);
}

/**
 * Manages the stack of expressions when a parenthesis is encountered.
 * @param bistromathique: The bistromathique structure.
 * @param expression_stack: The stack of expressions.
 * @param index: The current index in the arithmetic expression.
 * @return: The current parenthesis level in the expression.
 */
static int manage_parenthesis(t_bistromathique bistromathique, t_expression_stack **expression_stack, int index)
{
    static int expression_level = 0;

    if (bistromathique.expr[index] == bistromathique.ops[OP_OPEN_PARENT_IDX])
    {
        expression_level += 1;
        if (create_sub_expression(expression_stack, expression_level) == -1)
        {
            empty_expression_stack(expression_stack);
            return -1;
        }
    }
    else if (bistromathique.expr[index] == bistromathique.ops[OP_CLOSE_PARENT_IDX])
    {
        expression_level -= 1;
        if (merge_expressions(expression_stack) == -1)
        {
            empty_expression_stack(expression_stack);
            return -1;
        }
    }
    return expression_level;
}

/**
 * Parses the arithmetic expression in the bistromathique structure and turns it into an expression tree.
 * @param bistromathique: The bistromathique structure.
 * @return: The expression tree generated.
 */
static t_expression_tree *parse_expr(t_bistromathique bistromathique)
{
    t_expression_tree *expression_node = NULL;
    t_expression_stack *expression_stack = NULL;
    int expression_level = 0;
    int index = 0;

    if (create_sub_expression(&expression_stack, expression_level) == -1)
        return NULL;
    while (index < bistromathique.size)
    {
        if (is_parenthesis(bistromathique, bistromathique.expr[index]))
        {
            if ((expression_level = manage_parenthesis(bistromathique, &expression_stack, index)) == -1)
                return NULL;
        }
        else if (is_operator(bistromathique, bistromathique.expr[index]))
        {
            if ((expression_node = create_expression(bistromathique, index, expression_level)) == NULL)
            {
                empty_expression_stack(&expression_stack);
                return NULL;
            }
            update_root_expression(bistromathique, &expression_stack->expression_root, expression_node);
        }
        index += 1;
    }
    return expression_stack->expression_root;
}

/**
 * Initializes the bistomathique variables within the structure.
 * @param base: The list of the base digits represented as a string.
 * @param ops: The list of operators represented as a string.
 * @param expr: The arithmetic expression to compute.
 * @param size: The size of the expression.
 * @return: The bistromathique structure.
 */
static t_bistromathique init_bistromathique(char *base, char *ops, char *expr, unsigned int size)
{
    t_bistromathique bistromathique;

    bistromathique.base = base;
    bistromathique.base_length = my_strlen(base);
    bistromathique.ops = ops;
    bistromathique.expr = expr;
    bistromathique.size = size;
    return bistromathique;
}

/**
 * Evaluates the expression provided.
 * @param base: The list of the base digits represented as a string.
 * @param ops: The list of operators represented as a string.
 * @param expr: The arithmetic expression to compute.
 * @param size: The size of the expression.
 * @return
 */
char *eval_expr(char *base, char *ops, char *expr, unsigned int size)
{
    t_bistromathique bistromathique = init_bistromathique(base, ops, expr, size);
    t_expression_tree *expression_root = NULL;
    t_operation_list *operation_list = NULL;
    t_number *result = NULL;

    if ((operation_list = init_operation_list(bistromathique)) == NULL)
        return NULL;
    if ((expression_root = parse_expr(bistromathique)) == NULL)
    {
        empty_operation_list(operation_list);
        return NULL;
    }
    result = compute(bistromathique, operation_list, &expression_root);
    if (result->sign == SIGN_NEG)
        result->value = str_prepend(result->value, bistromathique.ops[OP_NEG_IDX], result->size);
    return result->value;
}
