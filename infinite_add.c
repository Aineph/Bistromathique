/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number perform_addition(t_bistromathique bistromathique, t_number nb_a, t_number nb_b, t_number result)
{
    int offset = result.size;
    char ret = bistromathique.base[0];
    int tmp_result = 0;
    int position = 1;

    result.value[offset--] = '\0';
    while (nb_a.size - position >= 0 || nb_b.size - position >= 0)
    {
        tmp_result = 0;
        if (nb_a.size - position >= 0)
            tmp_result += get_value(bistromathique, nb_a.value[nb_a.size - position]);
        if (nb_b.size - position >= 0)
            tmp_result += get_value(bistromathique, nb_b.value[nb_b.size - position]);
        tmp_result += get_value(bistromathique, ret);
        ret = bistromathique.base[tmp_result / bistromathique.base_length];
        result.value[offset--] = bistromathique.base[tmp_result % bistromathique.base_length];
        position += 1;
    }
    if (ret != bistromathique.base[0])
    {
        result.value = str_prepend(result.value, ret);
        result.size += 1;
    }
    return result;
}

t_number simple_add(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result = create_number();

    if (nb_a.value == NULL)
        return assign_value_to_number(nb_b.value, nb_b.size);
    if (nb_b.value == NULL)
        return assign_value_to_number(nb_a.value, nb_b.size);
    if (is_higher(bistromathique, nb_a, nb_b))
        result.size = nb_a.size;
    else
        result.size = nb_b.size;
    if ((result.value = malloc(sizeof(*result.value) * (result.size + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        result.size = 0;
        return result;
    }
    return perform_addition(bistromathique, nb_a, nb_b, result);
}

t_number infinite_add(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result;

    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        nb_a = number_to_positive(bistromathique, nb_a);
        nb_b = number_to_positive(bistromathique, nb_b);
        result = number_to_negative(bistromathique, simple_add(bistromathique, nb_a, nb_b));
    }
    else if (is_negative(bistromathique, nb_a) || is_negative(bistromathique, nb_b))
    {
        if (is_higher(bistromathique, nb_a, nb_b))
        {
            nb_b = number_to_positive(bistromathique, nb_b);
            result = infinite_sub(bistromathique, nb_a, nb_b);
        }
        else
        {
            nb_a = number_to_positive(bistromathique, nb_a);
            result = infinite_sub(bistromathique, nb_b, nb_a);
        }
    }
    else
        result = simple_add(bistromathique, nb_a, nb_b);
    return result;
}