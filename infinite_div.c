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
    int norm;

    tmp = create_number();
    norm = bistromathique.base_length / (get_value(bistromathique, nb_b->value[position_b]) + 1);
    printf("%d\n", norm);
    if (norm != 1)
    {
        printf("nb_a = %s\n", nb_a->value);
        assign_value_to_number(tmp, &bistromathique.base[norm % bistromathique.base_length], 1, SIGN_POS);
        printf("tmp = %s\n", tmp->value);
        res_tmp = simple_mul(bistromathique, nb_a, tmp);
        assign_value_to_number(nb_a, res_tmp->value, res_tmp->size, res_tmp->sign);
        printf("nb_a = %s\n", nb_a->value);
        free_number(res_tmp);
        res_tmp = simple_mul(bistromathique, nb_b, tmp);
        assign_value_to_number(nb_b, res_tmp->value, res_tmp->size, res_tmp->sign);
        free_number(res_tmp);
        printf("nb_b = %s\n", nb_b->value);
    }
    if ((nb_a->value = str_prepend(nb_a->value, bistromathique.base[0], nb_a->size)) == NULL)
        return NULL;
    nb_a->size += 1;
    printf("%s, %s\n", nb_a->value, nb_b->value);
    while (offset <= result->size)
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
            printf("new nb_a = %s\n", nb_a->value);
            free_number(res_tmp);
        }
        if (negativity == 1)
        {
            printf("negativity\n");
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
        printf("guess = %d\n", guess);
        printf("offset = %d\n", offset);
        result->value[offset] = bistromathique.base[guess % bistromathique.base_length];
        offset += 1;
    }
    printf("%d\n", offset);
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
    // result->value[result->size] = '\0';
    return result;
}