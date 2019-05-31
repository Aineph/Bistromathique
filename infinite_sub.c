/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *perform_substraction(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = result->size;
    int ret = 0;
    int tmp_result = 0;
    int position = 1;

    result->value[offset--] = '\0';
    while (nb_a->size - position >= 0)
    {
        tmp_result = nb_a->value[nb_a->size - position];
        if (nb_b->size - position >= 0)
            tmp_result -= nb_b->value[nb_b->size - position];
        tmp_result -= ret;
        if (tmp_result < 0)
        {
            tmp_result += bistromathique.base_length;
            ret = 1;
        }
        else
            ret = 0;
        result->value[offset--] = tmp_result % bistromathique.base_length;
        position += 1;
    }
    if (epur_result(bistromathique, result) == -1)
        return NULL;
    return result;
}

t_number *simple_sub(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if ((result = create_number()) == NULL)
        return NULL;
    if (is_null(nb_a))
    {
        if (copy_number(result, nb_b->value, nb_b->size, SIGN_NEG) == -1)
            return NULL;
        return result;
    }
    if (is_null(nb_b))
    {
        if (copy_number(result, nb_a->value, nb_a->size, nb_a->sign) == -1)
            return NULL;
        return result;
    }
    result->size = nb_a->size;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        result->size = 0;
        return result;
    }
    return perform_substraction(bistromathique, nb_a, nb_b, result);
}

t_number *infinite_sub(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if (is_negative(nb_b))
    {
        nb_b->sign = SIGN_POS;
        result = infinite_add(bistromathique, nb_a, nb_b);
        nb_b->sign = SIGN_NEG;
    }
    else if (is_higher(nb_b, nb_a))
    {
        if (is_negative(nb_a))
        {
            nb_a->sign = SIGN_POS;
            result = infinite_add(bistromathique, nb_a, nb_b);
            nb_a->sign = SIGN_NEG;
        }
        else
            result = simple_sub(bistromathique, nb_b, nb_a);
        result->sign = SIGN_NEG;
    }
    else
        result = simple_sub(bistromathique, nb_a, nb_b);
    return result;
}
