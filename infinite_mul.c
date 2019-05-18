/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

void free_multiplication(t_multiplication multiplication)
{
    free_number(multiplication.high_a);
    free_number(multiplication.low_a);
    free_number(multiplication.high_b);
    free_number(multiplication.low_b);
    free_number(multiplication.a0);
    free_number(multiplication.a1);
    free_number(multiplication.a2);
}

t_multiplication init_multiplication(t_number *nb_a, t_number *nb_b)
{
    t_multiplication multiplication;

    multiplication.middle = (MAX(nb_a->size, nb_b->size) + 1) / 2;
    multiplication.high_a = create_number();
    multiplication.low_a = create_number();
    multiplication.high_b = create_number();
    multiplication.low_b = create_number();
    multiplication.a0 = create_number();
    multiplication.a1 = create_number();
    multiplication.a2 = create_number();
    if (nb_a->size <= multiplication.middle)
    {
        assign_value_to_number(multiplication.high_a, "0", 1);
        assign_value_to_number(multiplication.low_a, nb_a->value, nb_a->size);
    }
    else
    {
        assign_value_to_number(multiplication.high_a, nb_a->value, nb_a->size - multiplication.middle);
        assign_value_to_number(multiplication.low_a, &nb_a->value[nb_a->size - multiplication.middle],
                               multiplication.middle);
    }
    if (nb_b->size <= multiplication.middle)
    {
        assign_value_to_number(multiplication.high_b, "0", 1);
        assign_value_to_number(multiplication.low_b, nb_b->value, 1);
    }
    else
    {
        assign_value_to_number(multiplication.high_b, nb_b->value, nb_b->size - multiplication.middle);
        assign_value_to_number(multiplication.low_b, &nb_b->value[nb_b->size - multiplication.middle],
                               multiplication.middle);
    }
    return multiplication;
}

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
    epur_result(bistromathique, result);
    return result;
}

t_number *simple_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_multiplication multiplication;
    t_number *result;
    t_number *tmp_result;
    t_number *a1_a;
    t_number *a1_b;

    if (nb_a->size + nb_b->size < 100 || nb_a->size < 25 || nb_b->size < 25)
    {
        result = create_number();
        result->size = nb_a->size + nb_b->size;
        if ((result->value = malloc(sizeof(*result) * (result->size + 1))) == NULL)
            return NULL;
        return perform_multiplication(bistromathique, nb_a, nb_b, result);
    }
    multiplication = init_multiplication(nb_a, nb_b);
    if (is_null(bistromathique, multiplication.low_a) || is_null(bistromathique, multiplication.low_b))
        assign_value_to_number(multiplication.a2, "0", 1);
    else
        multiplication.a2 = simple_mul(bistromathique, multiplication.low_a, multiplication.low_b);
    a1_a = simple_add(bistromathique, multiplication.low_a, multiplication.high_a);
    a1_b = simple_add(bistromathique, multiplication.low_b, multiplication.high_b);
    if (is_null(bistromathique, a1_a) || is_null(bistromathique, a1_b))
        assign_value_to_number(multiplication.a1, "0", 1);
    else
        multiplication.a1 = simple_mul(bistromathique, a1_a, a1_b);
    if (is_null(bistromathique, multiplication.high_a) || is_null(bistromathique, multiplication.high_b))
        assign_value_to_number(multiplication.a2, "0", 1);
    else
        multiplication.a0 = simple_mul(bistromathique, multiplication.high_a, multiplication.high_b);
    result = simple_sub(bistromathique, multiplication.a1, multiplication.a0);
    multiplication.a1 = simple_sub(bistromathique, result, multiplication.a2);
    free_number(result);
    multiplication.a0->value = str_rpad(multiplication.a0->value, multiplication.a0->size, bistromathique.base[0],
                                        multiplication.middle + multiplication.middle);
    multiplication.a0->size += (multiplication.middle + multiplication.middle);
    multiplication.a1->value = str_rpad(multiplication.a1->value, multiplication.a1->size, bistromathique.base[0],
                                        multiplication.middle);
    multiplication.a1->size += multiplication.middle;
    tmp_result = simple_add(bistromathique, multiplication.a0, multiplication.a1);
    result = simple_add(bistromathique, tmp_result, multiplication.a2);
    free_number(tmp_result);
    free_number(a1_a);
    free_number(a1_b);
    free_multiplication(multiplication);
    return result;
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