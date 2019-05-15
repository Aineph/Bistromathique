/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *perform_multiplication(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = result->size;
    int tmp_result = 0;
    int ret = 0;
    int position_a;
    int position_b;
    
    result->value[offset--] = '\0';
    while (offset >= 0)
    {
        tmp_result = ret;
        position_b = result->size - offset;
        position_a = 1;
        if (position_b > nb_b->size)
        {
            position_a += position_b - nb_b->size;
            position_b -= position_b - nb_b->size;
        }
        while (position_a <= result->size - offset && position_a <= nb_a->size)
        {
            tmp_result += (get_value(bistromathique, nb_a->value[nb_a->size - position_a]) *
                           get_value(bistromathique, nb_b->value[nb_b->size - position_b]));
            position_a += 1;
            position_b -= 1;
        }
        result->value[offset] = bistromathique.base[tmp_result % bistromathique.base_length];
        ret = tmp_result / bistromathique.base_length;
        offset -= 1;
    }
    return result;
}

t_number *simple_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result;

    if ((result = create_number()) == NULL)
        return NULL;
    if (nb_a->value == NULL || nb_b->value == NULL)
    {
        free_number(result);
        return NULL;
    }
    if (is_higher(bistromathique, nb_a, nb_b))
        result->size = nb_a->size * 2;
    else
        result->size = nb_b->size * 2;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_number(result);
        return NULL;
    }
    return perform_multiplication(bistromathique, nb_a, nb_b, result);
}

t_number *infinite_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result;

    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_a) == -1)
            return NULL;
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
    }
    else if (is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_a) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
    }
    else if (!is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
    }
    else
        result = simple_mul(bistromathique, nb_a, nb_b);
    if (epur_result(bistromathique, result) == -1)
        return NULL;
    return result;
}