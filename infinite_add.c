/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

char *simple_add(t_bistromathique bistromathique, char *nb_a, char *nb_b)
{
    char *result = NULL;
    int base_length = my_strlen(bistromathique.base);
    char ret = bistromathique.base[0];
    int tmp_result = 0;
    int nb_a_length = my_strlen(nb_a);
    int nb_b_length = my_strlen(nb_b);
    int position = 1;
    int offset = 0;

    if (is_higher(bistromathique, nb_a, nb_b))
        offset = nb_a_length;
    else
        offset = nb_b_length;
    if ((result = malloc((sizeof(*result) * offset) + 1)) == NULL)
        return result;
    result[offset--] = '\0';
    while (nb_a_length - position >= 0 || nb_b_length - position >= 0)
    {
        tmp_result = 0;
        if (nb_a_length - position >= 0)
            tmp_result += get_value(bistromathique, nb_a[nb_a_length - position]);
        if (nb_b_length - position >= 0)
            tmp_result += get_value(bistromathique, nb_b[nb_b_length - position]);
        tmp_result += get_value(bistromathique, ret);
        ret = bistromathique.base[tmp_result / base_length];
        result[offset--] = bistromathique.base[tmp_result % base_length];
        position += 1;
    }
    if (ret != bistromathique.base[0])
        result = concat_before(result, ret);
    return result;
}

char *infinite_add(t_bistromathique bistromathique, char *nb_a, char *nb_b)
{
    char *result = NULL;

    result = simple_add(bistromathique, nb_a, nb_b);
    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
        result = concat_before(result, bistromathique.ops[OP_NEG_IDX]);
    return result;
}