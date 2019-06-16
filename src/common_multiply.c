/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-06-08.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Subtracts a given number to the multiplication result.
 * @param multiplication: The multiplication structure.
 * @param number: The number to subtract.
 * @param offset: The number of zeros at the end of the given number.
 */
static void add_negative_to_result(t_multiplication *multiplication, t_number *number, int offset)
{
    int position = 1;

    multiplication->carry = 0;
    while (position <= number->size && position + offset <= multiplication->result->size)
    {
        multiplication->result->value[multiplication->result->size - offset - position] -=
                number->value[number->size - position] + multiplication->carry;
        multiplication->carry = 0;
        if (multiplication->result->value[multiplication->result->size - offset - position] < 0)
        {
            multiplication->result->value[multiplication->result->size - offset - position] += multiplication->base;
            multiplication->carry = 1;
        }
        position += 1;
    }
    while (multiplication->carry == 1 && position + offset <= multiplication->result->size)
    {
        multiplication->result->value[multiplication->result->size - offset - position] -= multiplication->carry;
        multiplication->carry = 0;
        if (multiplication->result->value[multiplication->result->size - offset - position] < 0)
        {
            multiplication->result->value[multiplication->result->size - offset - position] += multiplication->base;
            multiplication->carry = 1;
        }
        position += 1;
    }
}

/**
 * Adds a given number to the multiplication result.
 * @param multiplication: The multiplication structure.
 * @param number: The number to add.
 * @param offset: The number of zeros at the end of the given number.
 */
static void add_positive_to_result(t_multiplication *multiplication, t_number *number, int offset)
{
    int position = 1;

    multiplication->carry = 0;
    while (position <= number->size && position + offset <= multiplication->result->size)
    {
        multiplication->result->value[multiplication->result->size - offset - position] +=
                number->value[number->size - position] + multiplication->carry;
        multiplication->carry = 0;
        if (multiplication->result->value[multiplication->result->size - offset - position] > multiplication->base - 1)
        {
            multiplication->result->value[multiplication->result->size - offset - position] -= multiplication->base;
            multiplication->carry = 1;
        }
        position += 1;
    }
    while (multiplication->carry == 1 && position + offset <= multiplication->result->size)
    {
        multiplication->result->value[multiplication->result->size - offset - position] += multiplication->carry;
        multiplication->carry = 0;
        if (multiplication->result->value[multiplication->result->size - offset - position] > multiplication->base - 1)
        {
            multiplication->result->value[multiplication->result->size - offset - position] -= multiplication->base;
            multiplication->carry = 1;
        }
        position += 1;
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
    multiplication->a1_a = infinite_sub(bistromathique, multiplication->high_a, multiplication->low_a);
    multiplication->a1_b = infinite_sub(bistromathique, multiplication->high_b, multiplication->low_b);
    if ((is_null(multiplication->high_a) || is_null(multiplication->high_b)))
    {
        if ((multiplication->a0 = create_number()) == NULL)
            return -1;
    }
    else
        multiplication->a0 = infinite_mul(bistromathique, multiplication->high_a, multiplication->high_b);
    if ((is_null(multiplication->a1_a) || is_null(multiplication->a1_b)))
    {
        if ((multiplication->a1 = create_number()) == NULL)
            return -1;
    }
    else
        multiplication->a1 = infinite_mul(bistromathique, multiplication->a1_a, multiplication->a1_b);
    if ((is_null(multiplication->low_a) || is_null(multiplication->low_b)))
    {
        if ((multiplication->a2 = create_number()) == NULL)
            return -1;
    }
    else
        multiplication->a2 = infinite_mul(bistromathique, multiplication->low_a, multiplication->low_b);
    multiplication->a1->sign = SIGN_NEG;
    if (multiplication->a1_a->sign != multiplication->a1_b->sign)
        multiplication->a1->sign = SIGN_POS;
    return 0;
}

/**
 * Initializes the values to perform the multiplication.
 * @param multiplication: The multiplication structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 */
void init_multiplication_values(t_multiplication *multiplication, t_number *nb_a, t_number *nb_b)
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
 * Retrieves the result of the recursive multiplication algorithm.
 * @param bistromathique: The bistromathique structure.
 * @param multiplication: The multiplication structure.
 */
int process_multiplication(t_bistromathique bistromathique, t_multiplication *multiplication)
{
    if (compute_multiplication_values(bistromathique, multiplication) == -1)
        return -1;
    fill_number_with_zeros(multiplication->result);
    add_positive_to_result(multiplication, multiplication->a0, multiplication->middle + multiplication->middle);
    add_positive_to_result(multiplication, multiplication->a0, multiplication->middle);
    if (is_negative(multiplication->a1))
        add_negative_to_result(multiplication, multiplication->a1, multiplication->middle);
    else
        add_positive_to_result(multiplication, multiplication->a1, multiplication->middle);
    add_positive_to_result(multiplication, multiplication->a2, multiplication->middle);
    add_positive_to_result(multiplication, multiplication->a2, 0);
    return 0;
}
