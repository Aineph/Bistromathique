/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_negative(t_bistromathique bistromathique, t_number *number)
{
    if (number->value[0] == bistromathique.ops[OP_NEG_IDX])
        return 1;
    return 0;
}

int is_higher(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    int i = 0;

    if ((!is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
        (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && nb_b->size > nb_a->size) ||
        (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && nb_a->size > nb_b->size))
        return 1;
    else if ((is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
             (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && nb_b->size < nb_a->size) ||
             (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && nb_a->size < nb_b->size))
        return 0;
    while (i < nb_a->size)
    {
        if ((is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a->value[i]) < get_value(bistromathique, nb_b->value[i])) ||
            (!is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a->value[i]) > get_value(bistromathique, nb_b->value[i])))
            return 1;
        else if ((is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a->value[i]) > get_value(bistromathique, nb_b->value[i])) ||
                 (!is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a->value[i]) < get_value(bistromathique, nb_b->value[i])))
            return 0;
        i += 1;
    }
    return 0;
}

int epur_result(t_bistromathique bistromathique, t_number *result)
{
    int epur_index = 0;

    while (epur_index < result->size && result->value[epur_index] == bistromathique.base[0])
        epur_index += 1;
    if ((result->value = str_slice(result->value, epur_index, result->size)) == NULL)
        return -1;
    result->size -= epur_index;
    return 0;
}
