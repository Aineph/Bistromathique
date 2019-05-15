/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

int epur_sub_result(t_bistromathique bistromathique, t_number *result)
{
    int epur_index = 0;

    while (epur_index < result->size && result->value[epur_index] == bistromathique.base[0])
        epur_index += 1;
    if ((result->value = str_slice(result->value, epur_index, result->size)) == NULL)
        return -1;
    result->size -= epur_index;
    return 0;
}

t_number *perform_substraction(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = result->size;
    char ret = bistromathique.base[0];
    int tmp_result = 0;
    int position = 1;

    result->value[offset--] = '\0';
    while (nb_a->size - position >= 0)
    {
        tmp_result = get_value(bistromathique, nb_a->value[nb_a->size - position]);
        if (nb_b->size - position >= 0)
            tmp_result -= get_value(bistromathique, nb_b->value[nb_b->size - position]);
        tmp_result -= get_value(bistromathique, ret);
        if (tmp_result < 0)
        {
            tmp_result += bistromathique.base_length;
            ret = bistromathique.base[1];
        }
        else
            ret = bistromathique.base[0];
        result->value[offset--] = bistromathique.base[tmp_result % bistromathique.base_length];
        position += 1;
    }
    return result;
}

t_number *simple_sub(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if ((result = create_number()) == NULL)
        return NULL;
    if (nb_a->value == NULL)
    {
        if (number_to_negative(bistromathique, nb_b) == -1)
            return NULL;
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

    if (is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = infinite_add(bistromathique, nb_a, nb_b);
    }
    else if (is_higher(bistromathique, nb_b, nb_a))
    {
        result = simple_sub(bistromathique, nb_b, nb_a);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
        return result;
    }
    else
        result = simple_sub(bistromathique, nb_a, nb_b);
    if (epur_sub_result(bistromathique, result) == -1)
        return NULL;
    return result;
}