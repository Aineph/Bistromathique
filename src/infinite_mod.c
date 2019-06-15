/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-27.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Computes the modulo of two infinite numbers.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The rest of the division of the two given numbers.
 */
t_number *infinite_mod(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;
    t_number *result_div = NULL;
    t_number *result_mul = NULL;

    if ((result_div = infinite_div(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    result_mul = infinite_mul(bistromathique, result_div, nb_b);
    free_number(result_div);
    result = infinite_sub(bistromathique, nb_a, result_mul);
    free_number(result_mul);
    return result;
}
