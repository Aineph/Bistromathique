/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include <stdio.h>
#include "bistromathique.h"

t_expression_tree *parse_expr(t_bistromathique bistromathique)
{
    t_expression_tree *expression_node = NULL;
    t_expression_list *expression_list = NULL;
    int element_size = 0;
    int i = 0;

    if (init_expression_in_list(&expression_list) == -1)
        return NULL;
    while (i < bistromathique.size)
    {
        if (bistromathique.ops[OP_OPEN_PARENT_IDX] == bistromathique.expr[i])
        {
            element_size = -1;
            if (init_expression_in_list(&expression_list) == -1)
                return NULL;
        }
        else if (bistromathique.ops[OP_CLOSE_PARENT_IDX] == bistromathique.expr[i])
        {
            element_size = -1;
            if (merge_expressions(&expression_list) == -1)
                return NULL;
        }
        else if (is_operator(bistromathique, bistromathique.expr[i]))
        {
            if ((expression_node = create_expression(bistromathique, i, element_size)) == NULL)
                return NULL;
            update_root_expression(bistromathique, &expression_list->expression_root, expression_node);
            element_size = -1;
        }
        element_size += 1;
        i += 1;
    }
    return expression_list->expression_root;
}

void print_expr(t_expression_tree *expression_node)
{
    if (expression_node->result.value != NULL)
    {
        printf("Value: %s\n", expression_node->result.value);
        return;
    }
    if (expression_node->first != NULL)
        print_expr(expression_node->first);
    printf("%c\n", expression_node->operator);
    if (expression_node->second != NULL)
        print_expr(expression_node->second);
}

char *eval_expr(char *base, char *ops, char *expr, unsigned int size)
{
    t_bistromathique bistromathique;
    t_expression_tree *expression_root;
    t_operation_list *operation_list = NULL;

    bistromathique.base = base;
    bistromathique.base_length = my_strlen(base);
    bistromathique.ops = ops;
    bistromathique.expr = expr;
    bistromathique.size = size;
    if ((operation_list = init_operation_list(bistromathique)) == NULL)
        return NULL;
    if ((expression_root = parse_expr(bistromathique)) == NULL)
    {
        free_operation_list(operation_list);
        return NULL;
    };
    return compute(bistromathique, operation_list, expression_root).value;
}
