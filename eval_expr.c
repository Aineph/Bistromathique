/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_expression *parse_expr(t_bistromathique bistromathique)
{
    t_expression *new_expression = NULL;
    t_expression_list *expression_list = NULL;
    int i = 0;

    if (init_expression_in_list(&expression_list) == -1)
        return NULL;
    while (i < bistromathique.size)
    {
        if (bistromathique.ops[OP_OPEN_PARENT_IDX] == bistromathique.expr[i])
        {
            if (init_expression_in_list(&expression_list) == -1)
                return NULL;
        }
        else if (bistromathique.ops[OP_CLOSE_PARENT_IDX] == bistromathique.expr[i])
        {
            if (merge_expressions(&expression_list) == -1)
                return NULL;
        }
        else if (is_operator(bistromathique.ops, bistromathique.expr[i]))
        {
            if ((new_expression = create_expression(bistromathique, i)) == NULL)
                return NULL;
            update_root_expression(bistromathique, &expression_list->expression, new_expression);
        }
        i += 1;
    }
//    printf("***********************\n");
  //  print_expr(expression_list->expression);
    return expression_list->expression;
}

void print_expr(t_expression *expression)
{
//    printf("----Entering New Node----\n", expression->result);
    if (expression->result != NULL)
    {
        printf("Value: %s\n", expression->result);
        return;
    }
 //   printf("%c\n", expression->operator);
    if (expression->first != NULL)
    {
   //     printf("----Entering SubNode 1----\n");
        print_expr(expression->first);
    }
    printf("%c\n", expression->operator);
    if (expression->second != NULL)
    {
  //      printf("----Entering SubNode 2----\n");
        print_expr(expression->second);
    }
 //   printf("----Exiting Node----\n");
}

char *compute_expr(t_bistromathique bistromathique)
{
    t_expression *root;
    t_operation *operations = NULL;
    char *result = NULL;

    if ((operations = init_operations(bistromathique)) == NULL)
        return NULL;
    if ((root = parse_expr(bistromathique)) == NULL)
    {
        free_operations(operations);
        return NULL;
    };
    // print_expr(root);
//    free_expression(&root);
    return compute(bistromathique, operations, root);
}

char *eval_expr(char *base, char *ops, char *expr, unsigned int size)
{
    t_bistromathique bistromathique;

    bistromathique.base = base;
    bistromathique.ops = ops;
    bistromathique.expr = expr;
    bistromathique.size = size;
    return compute_expr(bistromathique);
}
