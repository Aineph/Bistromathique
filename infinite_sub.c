/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number epur_sub_result(t_bistromathique bistromathique, t_number result)
{
    t_number epur_result = create_number();
    int index = 0;
    int epur_index = 0;

    while (index < result.size && result.value[index] == bistromathique.base[0])
        index += 1;
    epur_result.size = result.size - index;
    if ((epur_result.value = malloc(sizeof(*epur_result.value) * (epur_result.size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        result.size = 0;
        return result;
    }
    while (epur_index < epur_result.size)
    {
        epur_result.value[epur_index] = result.value[epur_index + index];
        epur_index += 1;
    }
    epur_result.value[epur_index] = '\0';
    destroy_number(result);
    return epur_result;
}

t_number perform_substraction(t_bistromathique bistromathique, t_number nb_a, t_number nb_b, t_number result)
{
    int offset = result.size;
    char ret = bistromathique.base[0];
    int tmp_result = 0;
    int position = 1;

    result.value[offset--] = '\0';
    while (nb_a.size - position >= 0)
    {
        tmp_result = get_value(bistromathique, nb_a.value[nb_a.size - position]);
        if (nb_b.size - position >= 0)
            tmp_result -= get_value(bistromathique, nb_b.value[nb_b.size - position]);
        tmp_result -= get_value(bistromathique, ret);
        if (tmp_result < 0)
        {
            tmp_result += bistromathique.base_length;
            ret = bistromathique.base[1];
        }
        else
            ret = bistromathique.base[0];
        result.value[offset--] = bistromathique.base[tmp_result % bistromathique.base_length];
        position += 1;
    }
    return result;
}

t_number simple_sub(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result = create_number();

    if (nb_a.value == NULL)
        return number_to_negative(bistromathique, nb_b);
    if (nb_b.value == NULL)
        return assign_value_to_number(nb_a.value, nb_a.size);
    result.size = nb_a.size;
    if ((result.value = malloc(sizeof(*result.value) * (result.size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        result.size = 0;
        return result;
    }
    return perform_substraction(bistromathique, nb_a, nb_b, result);
}

t_number infinite_sub(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result;

    if (is_negative(bistromathique, nb_b))
    {
        nb_b = number_to_positive(bistromathique, nb_b);
        result = infinite_add(bistromathique, nb_a, nb_b);
    }
    else if (is_higher(bistromathique, nb_b, nb_a))
        result = number_to_negative(bistromathique, simple_sub(bistromathique, nb_b, nb_a));
    else
        result = simple_sub(bistromathique, nb_a, nb_b);
    return epur_sub_result(bistromathique, result);
}