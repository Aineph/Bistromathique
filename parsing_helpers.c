/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_priority_operator(t_bistromathique bistromathique, char c)
{
    int i = 0;

    while (bistromathique.ops[i] != '\0')
    {
        if (c == bistromathique.ops[i] && (i == OP_MULT_IDX || i == OP_DIV_IDX || i == OP_MOD_IDX))
            return 1;
        i += 1;
    }
    return 0;
}

int is_operator(t_bistromathique bistromathique, char c)
{
    int i = 0;

    while (bistromathique.ops[i] != '\0')
    {
        if (c == bistromathique.ops[i])
            return 1;
        i += 1;
    }
    return 0;
}