/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-27.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *infinite_mod(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;
    t_number *result_div = NULL;
    t_number *result_mul = NULL;
    t_number *nb_a_save = NULL;

    nb_a_save = create_number();
    copy_number(nb_a, nb_a->value, nb_a->size, nb_a->sign);
    if ((result_div = infinite_div(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    result_mul = infinite_mul(bistromathique, result_div, nb_b);
    free_number(result_div);
    result = infinite_sub(bistromathique, nb_a_save, result_mul);
    free_number(result_mul);
    return result;
}
