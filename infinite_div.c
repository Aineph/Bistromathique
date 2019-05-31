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
            tmp_result += (nb_a->value[nb_a->size - position_a] * nb_b->value[nb_b->size - position_b]);
            position_a += 1;
            position_b -= 1;
        }
        result->value[offset] = tmp_result % bistromathique.base_length;
        ret = tmp_result / bistromathique.base_length;
        offset -= 1;
    }
    result->sign = SIGN_POS;
    if ((is_negative(nb_a) || is_negative(nb_b)) && nb_a->sign != nb_b->sign)
        result->sign = SIGN_NEG;
    return result;
}

int normalize_values(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *normalized_value = NULL;
    t_number *ratio_number = NULL;
    int ratio = bistromathique.base_length;

    if (nb_b->size > 0)
        ratio /= (nb_b->value[0] + 1);
    if (ratio != 1)
    {
        ratio_number = create_number();
        expr_to_number(bistromathique, ratio_number, &bistromathique.base[ratio % bistromathique.base_length], 1);
        normalized_value = simple_mul(bistromathique, nb_a, ratio_number);
        copy_number(nb_a, normalized_value->value, normalized_value->size, normalized_value->sign);
        free_number(normalized_value);
        normalized_value = simple_mul(bistromathique, nb_b, ratio_number);
        copy_number(nb_b, normalized_value->value, normalized_value->size, normalized_value->sign);
        free_number(normalized_value);
        free_number(ratio_number);
    }
    if ((nb_a->value = str_prepend(nb_a->value, 0, nb_a->size)) == NULL)
        return -1;
    nb_a->size += 1;
    return ratio;
}

int guess_quotient(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, int offset)
{
    int quotient = 0;

    if (nb_b->value[0] == nb_a->value[offset])
        quotient = bistromathique.base_length - 1;
    else
        quotient = (nb_a->value[offset] * bistromathique.base_length + nb_a->value[offset + 1]) / nb_b->value[0];
    if (nb_b->size > 1 && nb_b->value[1] * quotient >
                          (nb_a->value[offset] * 10 + nb_a->value[offset + 1] - nb_b->value[0]) * quotient *
                          bistromathique.base_length + nb_a->value[offset + 2])
    {
        quotient -= 1;
        if (nb_b->size > 1 && nb_b->value[1] * quotient >
                              (nb_a->value[offset] * 10 + nb_a->value[offset + 1] - nb_b->value[0] * quotient) *
                              bistromathique.base_length + nb_a->value[offset + 2])
            quotient -= 1;
    }
    return quotient;
}

int
multiply_and_subtract(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, int *quotient_guess, int offset)
{
    t_number *quotient = NULL;
    t_number *subtract = NULL;
    int difference = 0;
    int ret = 0;
    int i = 0;

    quotient = create_number();
    expr_to_number(bistromathique, quotient, &bistromathique.base[*quotient_guess % bistromathique.base_length], 1);
    subtract = multiply_div(bistromathique, nb_b, quotient);
    while (i < nb_b->size + 1)
    {

        difference = nb_a->value[offset + nb_b->size - i] - subtract->value[nb_b->size - i] - ret;
        if (difference < 0)
        {
            difference += bistromathique.base_length;
            ret = 1;
        }
        else
            ret = 0;
        nb_a->value[offset + nb_b->size - i] = difference % bistromathique.base_length;
        i += 1;
    }
    if (ret == 1)
        *quotient_guess -= 1;
    free_number(subtract);
    free_number(quotient);
    return ret;
}

int rollback(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, int offset)
{
    int sum = 0;
    int ret = 0;
    int i = 0;

    while (i < nb_b->size)
    {
        sum = nb_a->value[offset + nb_b->size - i] + nb_b->value[nb_b->size - 1 - i] + ret;
        if (sum > bistromathique.base_length - 1)
        {
            sum -= bistromathique.base_length;
            ret = 1;
        }
        else
            ret = 0;
        nb_a->value[offset + nb_b->size - i] = sum;
        i += 1;
    }
    if (ret == 1)
        nb_a->value[offset + nb_b->size - i] = (nb_a->value[offset + nb_b->size - i] + 1) % bistromathique.base_length;
    return 0;
}

t_number *simple_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = 0;
    int guess = 0;
    int ret = 0;
    int position = 0;
    int begin = 0;

    normalize_values(bistromathique, nb_a, nb_b);
    while (offset < nb_a->size - nb_b->size)
    {
        guess = guess_quotient(bistromathique, nb_a, nb_b, offset);
        if (guess != 0)
            ret = multiply_and_subtract(bistromathique, nb_a, nb_b, &guess, offset);
        if (ret == 1)
            rollback(bistromathique, nb_a, nb_b, offset);
        if (guess != 0)
            begin = 1;
        else if (!begin)
            position += 1;
        if (begin)
            result->value[offset - position] = guess % bistromathique.base_length;
        offset += 1;
    }
    result->size = offset - position;
    result->value[result->size] = '\0';
    return result;
}

t_number *infinite_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;
    t_sign nb_a_sign = nb_a->sign;
    t_sign nb_b_sign = nb_b->sign;

    if (is_null(nb_b))
    {
        my_putstr(ZERO_DIVISION_ERROR);
        return NULL;
    }
    if ((result = create_number()) == NULL)
        return NULL;
    if (nb_a_sign != nb_b_sign)
    {
        nb_a->sign = SIGN_POS;
        nb_b->sign = SIGN_POS;
        result->sign = SIGN_NEG;
    }
    if (is_null(nb_a) || (is_higher(nb_b, nb_a)))
    {
        expr_to_number(bistromathique, result, &bistromathique.base[0], 1);
        return result;
    }
    result->size = nb_a->size - nb_b->size + 1;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
    {
        free_number(result);
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result = simple_div(bistromathique, nb_a, nb_b, result);
    nb_a->sign = nb_a_sign;
    nb_b->sign = nb_b_sign;
    epur_result(bistromathique, result);
    return result;
}
