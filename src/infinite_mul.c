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
 * Performs the infinite multiplication.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 */
static void perform_multiplication(t_multiplication *multiplication, t_number *nb_a, t_number *nb_b)
{
    int offset = multiplication->result->size - 1;
    int position_a;
    int position_b;

    multiplication->product = 0;
    while (offset >= 0)
    {
        position_b = multiplication->result->size - offset;
        position_a = 1;
        if (position_b > nb_b->size)
        {
            position_a += (position_b - nb_b->size);
            position_b -= (position_b - nb_b->size);
        }
        while (position_a <= multiplication->result->size - offset && position_a <= nb_a->size)
            multiplication->product += (nb_a->value[nb_a->size - position_a++] *
                                        nb_b->value[nb_b->size - position_b--]);
        multiplication->result->value[offset] = multiplication->product % multiplication->base;
        multiplication->product = multiplication->product / multiplication->base;
        offset -= 1;
    }
}

/**
 * Allocates and initializes the variables of the multiplication structure.
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
        (multiplication->low_b = create_number()) == NULL)
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
    multiplication->result->size = nb_a->size + nb_b->size;
    if ((multiplication->result->value = malloc(
            sizeof(*multiplication->result->value) * multiplication->result->size)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_number(multiplication->result);
        free_multiplication(multiplication);
        return NULL;
    }
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

    if ((multiplication = create_multiplication(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    if (nb_a->size + nb_b->size < 120 || (nb_a->size < 30 || nb_b->size < 30))
        perform_multiplication(multiplication, nb_a, nb_b);
    else if (process_multiplication(bistromathique, multiplication) == -1)
    {
        free_number(multiplication->result);
        free_multiplication(multiplication);
        return NULL;
    }
    if (nb_a->sign != nb_b->sign)
        multiplication->result->sign = SIGN_NEG;
    result = multiplication->result;
    free_multiplication(multiplication);
    if (remove_leading_zeros(result) == -1)
    {
        free_number(result);
        return NULL;
    }
    return result;
}
