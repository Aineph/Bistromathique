/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-06-15.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Checks if the current expression has an operator or not.
 * @param bistromathique: The bistromathique structure.
 * @param position: The current position in the expression.
 * @param expression_level: The current parenthesis level in the expression.
 * @return: The expression containing the value of the number if the expression has no operator.
 */
t_expression_tree *expression_has_no_operator(t_bistromathique bistromathique, int position, int expression_level)
{
    t_expression_tree *expression_tree = NULL;
    int index = position;
    int is_parenthesis = 0;

    if (position >= 0 && bistromathique.expr[position] == bistromathique.ops[OP_OPEN_PARENT_IDX])
        is_parenthesis = 1;
    index += 1;
    while (index < bistromathique.size && !is_operator(bistromathique, bistromathique.expr[index]))
        index += 1;
    if ((is_parenthesis && bistromathique.expr[index] == bistromathique.ops[OP_CLOSE_PARENT_IDX]) ||
        (!is_parenthesis && bistromathique.expr[index] == '\0'))
    {
        if ((expression_tree = malloc(sizeof(*expression_tree))) == NULL)
        {
            my_putstr(MALLOC_ERROR);
            return NULL;
        }
        expression_tree->first = parse_right_value(bistromathique, position);
        expression_tree->second = parse_right_value(bistromathique, index);
        expression_tree->operator = bistromathique.ops[OP_PLUS_IDX];
        expression_tree->level = expression_level;
        expression_tree->result = NULL;
        return expression_tree;
    }
    return NULL;
}
