/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_negative(t_number *number)
{
    if (number != NULL && number->sign == SIGN_NEG)
        return 1;
    return 0;
}

int is_null(t_bistromathique bistromathique, t_number *number)
{
    int i = 0;

    if (number == NULL)
        return 1;
    if (number->value == NULL)
        return 1;
    while (i < number->size)
    {
        if (number->value[i] != bistromathique.base[0])
            return 0;
        i += 1;
    }
    return 1;
}

int is_higher(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    int i = 0;

    if ((!is_negative(nb_a) && is_negative(nb_b)) ||
        (is_negative(nb_a) && is_negative(nb_b) && nb_b->size > nb_a->size) ||
        (!is_negative(nb_a) && !is_negative(nb_b) && nb_a->size > nb_b->size))
        return 1;
    else if ((is_negative(nb_a) && !is_negative(nb_b)) ||
             (is_negative(nb_a) && is_negative(nb_b) && nb_b->size < nb_a->size) ||
             (!is_negative(nb_a) && !is_negative(nb_b) && nb_a->size < nb_b->size))
        return 0;
    while (i < nb_a->size)
    {
        if ((is_negative(nb_a) &&
             get_value(bistromathique, nb_a->value[i]) < get_value(bistromathique, nb_b->value[i])) ||
            (!is_negative(nb_a) &&
             get_value(bistromathique, nb_a->value[i]) > get_value(bistromathique, nb_b->value[i])))
            return 1;
        else if ((is_negative(nb_a) &&
                  get_value(bistromathique, nb_a->value[i]) > get_value(bistromathique, nb_b->value[i])) ||
                 (!is_negative(nb_a) &&
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
    if (epur_index > 0 && (result->value = str_slice(result->value, epur_index, result->size)) == NULL)
        return -1;
    result->size -= epur_index;
    if (result->size == 0 && epur_index > 0)
        assign_value_to_number(result, "0", 1, SIGN_POS);
    return 0;
}
