/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_priority_operator(t_bistromathique bistromathique, char c)
{
    int index = 0;

    while (bistromathique.ops[index] != '\0')
    {
        if (c == bistromathique.ops[index] && (index == OP_MULT_IDX || index == OP_DIV_IDX || index == OP_MOD_IDX))
            return 1;
        index += 1;
    }
    return 0;
}

int is_operator(t_bistromathique bistromathique, char c)
{
    int index = 0;

    while (bistromathique.ops[index] != '\0')
    {
        if (c == bistromathique.ops[index])
            return 1;
        index += 1;
    }
    return 0;
}