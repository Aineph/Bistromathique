/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include <stdio.h>
#include "bistromathique.h"

/*
 * To remove after finishing development.
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
 * Parses the arithmetic expression in the bistromathique structure and turns it into an expression tree.
 * @param bistromathique: The bistromathique structure.
 * @return: The expression tree generated.
 */
static t_expression_tree *parse_expr(t_bistromathique bistromathique)
{
    t_expression_tree *expression_node = NULL;
    t_expression_list *expression_list = NULL;
    int element_size = 0;
    int level = 0;
    int i = 0;

    if (init_expression_in_list(&expression_list) == -1)
        return NULL;
    while (i < bistromathique.size)
    {
        if (bistromathique.ops[OP_OPEN_PARENT_IDX] == bistromathique.expr[i])
        {
            element_size = -1;
            level += 1;
            if (init_expression_in_list(&expression_list) == -1)
                return NULL;
        }
        else if (bistromathique.ops[OP_CLOSE_PARENT_IDX] == bistromathique.expr[i])
        {
            element_size = -1;
            level -= 1;
            if (merge_expressions(&expression_list) == -1)
                return NULL;
        }
        else if (is_operator(bistromathique, bistromathique.expr[i]))
        {
            if ((expression_node = create_expression(bistromathique, i, level, element_size)) == NULL)
                return NULL;
            update_root_expression(bistromathique, &expression_list->expression_root, expression_node);
            element_size = -1;
        }
        element_size += 1;
        i += 1;
    }
    return expression_list->expression_root;
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
