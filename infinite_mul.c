/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the content of the multiplication structure without removing the result.
 * @param multiplication: The multiplication structure.
 */
static void free_multiplication(t_multiplication *multiplication)
{
    if (multiplication->high_a != NULL)
        free(multiplication->high_a);
    if (multiplication->high_b != NULL)
        free(multiplication->high_b);
    if (multiplication->low_a != NULL)
        free(multiplication->low_a);
    if (multiplication->low_b != NULL)
        free(multiplication->low_b);
    if (multiplication->a1_a != NULL)
        free_number(multiplication->a1_a);
    if (multiplication->a1_b != NULL)
        free_number(multiplication->a1_b);
    if (multiplication->a0 != NULL)
        free_number(multiplication->a0);
    if (multiplication->a1 != NULL)
        free_number(multiplication->a1);
    if (multiplication->a2 != NULL)
        free_number(multiplication->a2);
    free(multiplication);
}

/**
 * Initializes the values to perform the multiplication.
 * @param multiplication: The multiplication structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 */
static void init_multiplication_values(t_multiplication *multiplication, t_number *nb_a, t_number *nb_b)
{
    multiplication->middle = (MAX(nb_a->size, nb_b->size) + 1) / 2;
    if (nb_a->size <= multiplication->middle)
        reference_number(multiplication->low_a, nb_a->value, nb_a->size, SIGN_POS);
    else
    {
        reference_number(multiplication->high_a, nb_a->value, nb_a->size - multiplication->middle, SIGN_POS);
        reference_number(multiplication->low_a, &nb_a->value[nb_a->size - multiplication->middle],
                         multiplication->middle, SIGN_POS);
    }
    if (nb_b->size <= multiplication->middle)
        reference_number(multiplication->low_b, nb_b->value, nb_b->size, SIGN_POS);
    else
    {
        reference_number(multiplication->high_b, nb_b->value, nb_b->size - multiplication->middle, SIGN_POS);
        reference_number(multiplication->low_b, &nb_b->value[nb_b->size - multiplication->middle],
                         multiplication->middle, SIGN_POS);
    }
}

/**
 * Computes the needed values for the multiplication algorithm.
 * @param bistromathique: The bistromathique structure.
 * @param multiplication: The multiplication structure.
 * @return: A negative value if an error occurs. Zero if the computation completes successfully.
 */
static int compute_multiplication_values(t_bistromathique bistromathique, t_multiplication *multiplication)
{
    t_number *addition = NULL;
    t_number *subtraction = NULL;

    multiplication->a1_a = infinite_sub(bistromathique, multiplication->high_a, multiplication->low_a);
    multiplication->a1_b = infinite_sub(bistromathique, multiplication->high_b, multiplication->low_b);
    if ((is_null(multiplication->high_a) || is_null(multiplication->high_b)) &&
        expr_to_number(bistromathique, multiplication->a0, &bistromathique.base[0], 1) == -1)
        return -1;
    else
        multiplication->a0 = infinite_mul(bistromathique, multiplication->high_a, multiplication->high_b);
    if ((is_null(multiplication->a1_a) || is_null(multiplication->a1_b)) &&
        expr_to_number(bistromathique, multiplication->a1, &bistromathique.base[0], 1) == -1)
        return -1;
    else
        multiplication->a1 = infinite_mul(bistromathique, multiplication->a1_a, multiplication->a1_b);
    if ((is_null(multiplication->low_a) || is_null(multiplication->low_b)) &&
        expr_to_number(bistromathique, multiplication->a2, &bistromathique.base[0], 1) == -1)
        return -1;
    else
        multiplication->a2 = infinite_mul(bistromathique, multiplication->low_a, multiplication->low_b);
    addition = infinite_add(bistromathique, multiplication->a0, multiplication->a2);
    subtraction = infinite_sub(bistromathique, addition, multiplication->a1);
    free_number(addition);
    multiplication->a1 = subtraction;
    return 0;
}

/**
 * Retrieves the result of the recursive multiplication algorithm.
 * @param bistromathique: The bistromathique structure.
 * @param multiplication: The multiplication structure.
 */
static void process_multiplication(t_bistromathique bistromathique, t_multiplication *multiplication)
{
    t_number *tmp = NULL;

    multiplication->a0->value = str_rpad(multiplication->a0->value, multiplication->a0->size, 0,
                                         multiplication->middle + multiplication->middle);
    multiplication->a0->size += multiplication->middle + multiplication->middle;
    multiplication->a1->value = str_rpad(multiplication->a1->value, multiplication->a1->size, 0,
                                         multiplication->middle);
    multiplication->a1->size += multiplication->middle;
    tmp = infinite_add(bistromathique, multiplication->a0, multiplication->a1);
    multiplication->result = infinite_add(bistromathique, tmp, multiplication->a2);
    free_number(tmp);
}

/**
 * Performs the infinite multiplication.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @param result: The number in which to store the result.
 */
static void perform_multiplication(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number *result)
{
    int offset = result->size - 1;
    int tmp_result = 0;
    int position_a;
    int position_b;

    while (offset >= 0)
    {
        position_b = result->size - offset;
        position_a = 1;
        if (position_b > nb_b->size)
        {
            position_a += (position_b - nb_b->size);
            position_b -= (position_b - nb_b->size);
        }
        while (position_a <= result->size - offset && position_a <= nb_a->size)
            tmp_result += (nb_a->value[nb_a->size - position_a++] * nb_b->value[nb_b->size - position_b--]);
        result->value[offset] = tmp_result % bistromathique.base_length;
        tmp_result = tmp_result / bistromathique.base_length;
        offset -= 1;
    }
    remove_leading_zeros(result);
    result->sign = SIGN_POS;
    if (nb_a->sign != nb_b->sign)
        result->sign = SIGN_NEG;
}

/*
 * Will be replaced soon.
 */
t_number *simple_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;

    result = create_number();
    result->size = nb_a->size + nb_b->size;
    if ((result->value = malloc(sizeof(*result->value) * result->size)) == NULL)
        return NULL;
    perform_multiplication(bistromathique, nb_a, nb_b, result);
    return result;
}

/**
 * Allocates and initializes the variables of the subtraction structure.
 * @param multiplication: The multiplication structure to initialize.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The initialized multiplication structure.
 */
static t_multiplication *
init_multiplication(t_multiplication *multiplication, t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    multiplication->result = NULL;
    multiplication->high_a = NULL;
    multiplication->high_b = NULL;
    multiplication->low_a = NULL;
    multiplication->low_b = NULL;
    multiplication->a1_a = NULL;
    multiplication->a1_b = NULL;
    multiplication->a0 = NULL;
    multiplication->a1 = NULL;
    multiplication->a2 = NULL;
    if ((multiplication->result = create_number()) == NULL || (multiplication->high_a = create_number()) == NULL ||
        (multiplication->high_b = create_number()) == NULL || (multiplication->low_a = create_number()) == NULL ||
        (multiplication->low_b = create_number()) == NULL || (multiplication->a1_a = create_number()) == NULL ||
        (multiplication->a1_b = create_number()) == NULL || (multiplication->a0 = create_number()) == NULL ||
        (multiplication->a1 = create_number()) == NULL || (multiplication->a2 = create_number()) == NULL)
    {
        free_number(multiplication->result);
        free_multiplication(multiplication);
        return NULL;
    }
    init_multiplication_values(multiplication, nb_a, nb_b);
    multiplication->base = bistromathique.base_length;
    return multiplication;
}

/**
 * Creates the multiplication structure that contains every needed variables.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The multiplication structure.
 */
static t_multiplication *create_multiplication(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_multiplication *multiplication = NULL;

    if ((multiplication = malloc(sizeof(*multiplication))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    if ((multiplication = init_multiplication(multiplication, bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    return multiplication;
}

/**
 * Computes the multiplication of two infinite numbers.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The product of the two given numbers.
 */
t_number *infinite_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_multiplication *multiplication = NULL;
    t_number *result = NULL;

    if (nb_a->size + nb_b->size < 120 || (nb_a->size < 30 || nb_b->size < 30))
        return simple_mul(bistromathique, nb_a, nb_b);
    if ((multiplication = create_multiplication(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    if (compute_multiplication_values(bistromathique, multiplication) == -1)
    {
        free_number(multiplication->result);
        free_multiplication(multiplication);
        return NULL;
    }
    process_multiplication(bistromathique, multiplication);
    if (nb_a->sign != nb_b->sign)
        multiplication->result->sign = SIGN_NEG;
    result = multiplication->result;
    free_multiplication(multiplication);
    return result;
}
