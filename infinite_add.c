/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number simple_add(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result = {NULL, 0};
    int base_length = my_strlen(bistromathique.base);
    char ret = bistromathique.base[0];
    int tmp_result = 0;
    int position = 1;
    int offset = 0;

    if (is_higher(bistromathique, nb_a, nb_b))
        offset = nb_a.size;
    else
        offset = nb_b.size;
    result.size = offset;
    if ((result.value = malloc((sizeof(*result.value) * offset) + 1)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return result;
    }
    result.value[offset--] = '\0';
    while (nb_a.size - position >= 0 || nb_b.size - position >= 0)
    {
        tmp_result = 0;
        if (nb_a.size - position >= 0)
            tmp_result += get_value(bistromathique, nb_a.value[nb_a.size - position]);
        if (nb_b.size - position >= 0)
            tmp_result += get_value(bistromathique, nb_b.value[nb_b.size - position]);
        tmp_result += get_value(bistromathique, ret);
        ret = bistromathique.base[tmp_result / base_length];
        result.value[offset--] = bistromathique.base[tmp_result % base_length];
        position += 1;
    }
    if (ret != bistromathique.base[0])
    {
        result.value = str_prepend(result.value, ret);
        result.size += 1;
    }
    return result;
}

t_number infinite_add(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    t_number result = simple_add(bistromathique, nb_a, nb_b);

    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
        result.value = str_prepend(result.value, bistromathique.ops[OP_NEG_IDX]);
    return result;
}