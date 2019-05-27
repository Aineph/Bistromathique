/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-19.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number *multiply_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result;
    int tmp_result = 0;
    int ret = 0;
    int position_a;
    int position_b;
    int offset = 0;

    result = create_number();
    result->size = nb_a->size + nb_b->size;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
        return NULL;
    offset = result->size;
    result->value[offset--] = '\0';
    offset = result->size;
    while (offset >= 0)
    {
        tmp_result = ret;
        position_b = result->size - offset;
        position_a = 1;
        if (position_b > nb_b->size)
        {
            position_a += (position_b - nb_b->size);
            position_b -= (position_b - nb_b->size);
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
    result->sign = SIGN_POS;
    if ((is_negative(nb_a) || is_negative(nb_b)) && nb_a->sign != nb_b->sign)
        number_to_negative(result);
    return result;
}

int normalize_values(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *normalized_value = NULL;
    t_number *ratio_number = NULL;
    int ratio = bistromathique.base_length;

    if (nb_b->size > 0)
        ratio /= (get_value(bistromathique, nb_b->value[0]) + 1);
    if (ratio != 1)
    {
        ratio_number = create_number();
        assign_value_to_number(ratio_number, &bistromathique.base[ratio % bistromathique.base_length], 1, SIGN_POS);
        normalized_value = simple_mul(bistromathique, nb_a, ratio_number);
        assign_value_to_number(nb_a, normalized_value->value, normalized_value->size, normalized_value->sign);
        free_number(normalized_value);
        normalized_value = simple_mul(bistromathique, nb_b, ratio_number);
        assign_value_to_number(nb_b, normalized_value->value, normalized_value->size, normalized_value->sign);
        free_number(normalized_value);
        free_number(ratio_number);
    }
    if ((nb_a->value = str_prepend(nb_a->value, bistromathique.base[0], nb_a->size)) == NULL)
        return -1;
    nb_a->size += 1;
    return 0;
}

t_number *simple_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    t_number *tmp = NULL;
    t_number *res_tmp = NULL;
    int tmp_result = 0;
    int offset = 0;
    int iterator = 0;
    int position_b = 0;
    int guess = 0;
    int ret;
    int negativity;

    tmp = create_number();
    normalize_values(bistromathique, nb_a, nb_b);
    while (offset < result->size)
    {
        if (get_value(bistromathique, nb_b->value[position_b]) == get_value(bistromathique, nb_a->value[offset]))
            guess = bistromathique.base_length - 1;
        else
            guess = (get_value(bistromathique, nb_a->value[offset]) * bistromathique.base_length +
                     get_value(bistromathique, nb_a->value[offset + 1])) /
                    get_value(bistromathique, nb_b->value[position_b]);
        if (nb_b->size > 1 && get_value(bistromathique, nb_b->value[position_b + 1]) * guess >
                              (get_value(bistromathique, nb_a->value[offset]) * 10 +
                               get_value(bistromathique, nb_a->value[offset + 1]) -
                               get_value(bistromathique, nb_b->value[position_b]) * guess) *
                              bistromathique.base_length + get_value(bistromathique, nb_a->value[offset + 2]))
        {
            guess -= 1;
            if (nb_b->size > 1 && get_value(bistromathique, nb_b->value[position_b + 1]) * guess >
                                  (get_value(bistromathique, nb_a->value[offset]) * 10 +
                                   get_value(bistromathique, nb_a->value[offset + 1]) -
                                   get_value(bistromathique, nb_b->value[position_b]) * guess) *
                                  bistromathique.base_length + get_value(bistromathique, nb_a->value[offset + 2]))
                guess -= 1;
        }
        negativity = 0;
        if (guess != 0)
        {
            assign_value_to_number(tmp, &bistromathique.base[guess % bistromathique.base_length], 1, SIGN_POS);
            res_tmp = multiply_div(bistromathique, nb_b, tmp);
            iterator = 0;
            while (iterator < nb_b->size + 1)
            {
                tmp_result = get_value(bistromathique, nb_a->value[offset + nb_b->size - iterator]) -
                             get_value(bistromathique, res_tmp->value[nb_b->size - iterator]) - negativity;
                if (tmp_result < 0)
                {
                    tmp_result += bistromathique.base_length;
                    negativity = 1;
                }
                else
                    negativity = 0;
                nb_a->value[offset + nb_b->size - iterator] = bistromathique.base[tmp_result %
                                                                                  bistromathique.base_length];
                iterator += 1;
            }
            free_number(res_tmp);
        }
        if (negativity == 1)
        {
            guess -= 1;
            ret = 0;
            iterator = 0;
            while (iterator < nb_b->size)
            {
                tmp_result = get_value(bistromathique, nb_a->value[offset + nb_b->size - iterator]) +
                             get_value(bistromathique, nb_b->value[nb_b->size - 1 - iterator]) + ret;
                if (tmp_result > bistromathique.base_length - 1)
                {
                    tmp_result -= bistromathique.base_length;
                    ret = 1;
                }
                else
                    ret = 0;
                nb_a->value[offset + nb_b->size - iterator] = tmp_result;
                iterator += 1;
            }
            if (ret == 1)
                nb_a->value[offset + nb_b->size - iterator] =
                        (get_value(bistromathique, nb_a->value[offset + nb_b->size - iterator]) + 1) %
                        bistromathique.base_length;
        }
        res_tmp = create_number();
        result->value[offset] = bistromathique.base[guess % bistromathique.base_length];
        offset += 1;
        assign_value_to_number(res_tmp, nb_a->value, nb_a->size, nb_a->sign);
        epur_result(bistromathique, res_tmp);
        if (is_higher(bistromathique, nb_b, res_tmp))
        {
            free_number(res_tmp);
            result->size = offset;
            break;
        }
        free_number(res_tmp);
    }
    free_number(tmp);
    result->value[offset] = '\0';
    return result;
}

t_number *infinite_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    if (is_null(bistromathique, nb_b))
        return NULL;
    if ((result = create_number()) == NULL)
        return NULL;
    if (is_null(bistromathique, nb_a) || (nb_a->sign == nb_b->sign && is_higher(bistromathique, nb_b, nb_a)))
    {
        assign_value_to_number(result, "0", 1, SIGN_POS);
        return result;
    }
    result->size = nb_a->size - nb_b->size + 2;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
    {
        free_number(result);
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result = simple_div(bistromathique, nb_a, nb_b, result);
    epur_result(bistromathique, result);
    return result;
}