/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *perform_addition(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = result->size;
    int ret = 0;
    int tmp_result = 0;
    int position = 1;

    result->value[offset--] = '\0';
    while (nb_a->size - position >= 0 || nb_b->size - position >= 0)
    {
        tmp_result = 0;
        if (nb_a->size - position >= 0)
            tmp_result += get_value(bistromathique, nb_a->value[nb_a->size - position]);
        if (nb_b->size - position >= 0)
            tmp_result += get_value(bistromathique, nb_b->value[nb_b->size - position]);
        tmp_result += ret;
        ret = tmp_result / bistromathique.base_length;
        result->value[offset--] = bistromathique.base[tmp_result % bistromathique.base_length];
        position += 1;
    }
    result->value[offset] = bistromathique.base[ret];
    if (epur_result(bistromathique, result) == -1)
        return NULL;
    return result;
}

t_number *simple_add(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if ((result = create_number()) == NULL)
        return NULL;
    if (nb_a->value == NULL)
    {
        if (assign_value_to_number(result, nb_b->value, nb_b->size) == -1)
            return NULL;
        return result;
    }
    if (nb_b->value == NULL)
    {
        if (assign_value_to_number(result, nb_a->value, nb_a->size) == -1)
            return NULL;
        return result;
    }
    result->size = MAX(nb_a->size, nb_b->size) + 1;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        result->size = 0;
        return result;
    }
    return perform_addition(bistromathique, nb_a, nb_b, result);
}

t_number *infinite_add(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_a) == -1)
            return NULL;
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = simple_add(bistromathique, nb_a, nb_b);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
    }
    else if (is_negative(bistromathique, nb_a) || is_negative(bistromathique, nb_b))
    {
        if (is_higher(bistromathique, nb_a, nb_b))
        {
            if (number_to_positive(bistromathique, nb_b) == -1)
                return NULL;
            result = simple_sub(bistromathique, nb_a, nb_b);
        }
        else
        {
            if (number_to_positive(bistromathique, nb_a) == -1)
                return NULL;
            result = simple_sub(bistromathique, nb_b, nb_a);
        }
    }
    else
        result = simple_add(bistromathique, nb_a, nb_b);
    return result;
}