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

t_multiplication init_multiplication(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
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
        assign_value_to_number(multiplication.high_a, &bistromathique.ops[0], 1, SIGN_POS);
        assign_value_to_number(multiplication.low_a, nb_a->value, nb_a->size, nb_a->sign);
    }
    else
    {
        assign_value_to_number(multiplication.high_a, nb_a->value, nb_a->size - multiplication.middle, nb_a->sign);
        assign_value_to_number(multiplication.low_a, &nb_a->value[nb_a->size - multiplication.middle],
                               multiplication.middle, nb_a->sign);
    }
    if (nb_b->size <= multiplication.middle)
    {
        assign_value_to_number(multiplication.high_b, &bistromathique.ops[0], 1, SIGN_POS);
        assign_value_to_number(multiplication.low_b, nb_b->value, nb_b->size, nb_b->sign);
    }
    else
    {
        assign_value_to_number(multiplication.high_b, nb_b->value, nb_b->size - multiplication.middle, nb_b->sign);
        assign_value_to_number(multiplication.low_b, &nb_b->value[nb_b->size - multiplication.middle],
                               multiplication.middle, nb_b->sign);
    }
    epur_result(bistromathique, multiplication.high_a);
    epur_result(bistromathique, multiplication.low_a);
    epur_result(bistromathique, multiplication.high_b);
    epur_result(bistromathique, multiplication.low_b);
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
    result->sign = SIGN_POS;
    if ((is_negative(nb_a) || is_negative(nb_b)) && nb_a->sign != nb_b->sign)
        number_to_negative(result);
    return result;
}

t_number *simple_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    result = create_number();
    result->size = nb_a->size + nb_b->size;
    if ((result->value = malloc(sizeof(*result->value) * (result->size + 1))) == NULL)
        return NULL;
    return perform_multiplication(bistromathique, nb_a, nb_b, result);
}

t_number *recursive_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_multiplication multiplication;
    t_number *result;
    t_number *tmp_result;
    t_number *a1_a;
    t_number *a1_b;

    if (nb_a->size + nb_b->size < 100 || (nb_a->size < 25 || nb_b->size < 25))
        return simple_mul(bistromathique, nb_a, nb_b);
    multiplication = init_multiplication(bistromathique, nb_a, nb_b);
    a1_a = infinite_sub(bistromathique, multiplication.high_a, multiplication.low_a);
    a1_b = infinite_sub(bistromathique, multiplication.high_b, multiplication.low_b);
    if (is_null(bistromathique, multiplication.high_a) || is_null(bistromathique, multiplication.high_b))
        assign_value_to_number(multiplication.a0, &bistromathique.ops[0], 1, SIGN_POS);
    else
        multiplication.a0 = recursive_mul(bistromathique, multiplication.high_a, multiplication.high_b);
    if (is_null(bistromathique, a1_a) || is_null(bistromathique, a1_b))
        assign_value_to_number(multiplication.a1, &bistromathique.ops[0], 1, SIGN_POS);
    else
        multiplication.a1 = recursive_mul(bistromathique, a1_a, a1_b);
    if (is_null(bistromathique, multiplication.low_a) || is_null(bistromathique, multiplication.low_b))
        assign_value_to_number(multiplication.a2, &bistromathique.ops[0], 1, SIGN_POS);
    else
        multiplication.a2 = recursive_mul(bistromathique, multiplication.low_a, multiplication.low_b);
    tmp_result = infinite_add(bistromathique, multiplication.a0, multiplication.a2);
    result = infinite_sub(bistromathique, tmp_result, multiplication.a1);
    free_number(tmp_result);
    multiplication.a0->value = str_rpad(multiplication.a0->value, multiplication.a0->size, bistromathique.base[0],
                                        multiplication.middle + multiplication.middle);
    multiplication.a0->size += (multiplication.middle + multiplication.middle);
    result->value = str_rpad(result->value, result->size, bistromathique.base[0], multiplication.middle);
    result->size += multiplication.middle;
    tmp_result = infinite_add(bistromathique, multiplication.a0, result);
    result = infinite_add(bistromathique, tmp_result, multiplication.a2);
    free_number(tmp_result);
    free_number(a1_a);
    free_number(a1_b);
    free_multiplication(multiplication);
    return result;
}

t_number *infinite_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result;

    result = recursive_mul(bistromathique, nb_a, nb_b);
    return result;
}
