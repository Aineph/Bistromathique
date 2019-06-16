/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-06-08.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Guesses the quotient value.
 * @param division: The division structure.
 * @param offset: The current position in the result.
 */
void guess_quotient(t_division *division, int offset)
{
    if (division->normalized_b->value[0] == division->normalized_a->value[offset])
        division->quotient = division->base - 1;
    else
        division->quotient =
                (division->normalized_a->value[offset] * division->base + division->normalized_a->value[offset + 1]) /
                division->normalized_b->value[0];
    if (division->normalized_b->size > 1 && division->normalized_b->value[1] * division->quotient >
                                            (division->normalized_a->value[offset] * 10 +
                                             division->normalized_a->value[offset + 1] -
                                             division->normalized_b->value[0]) * division->quotient * division->base +
                                            division->normalized_a->value[offset + 2])
    {
        division->quotient -= 1;
        if (division->normalized_b->size > 1 && division->normalized_b->value[1] * division->quotient >
                                                (division->normalized_a->value[offset] * 10 +
                                                 division->normalized_a->value[offset + 1] -
                                                 division->normalized_b->value[0] * division->quotient) *
                                                division->base + division->normalized_a->value[offset + 2])
            division->quotient -= 1;
    }
}

/**
 * Manages the carry after multiplying and subtracting.
 * @param division: The division structure.
 * @param offset: The current position in the result.
 */
void rollback(t_division *division, int offset)
{
    int sum = 0;
    int ret = 0;
    int i = 0;

    while (i < division->normalized_b->size)
    {
        sum = division->normalized_a->value[offset + division->normalized_b->size - i] +
              division->normalized_b->value[division->normalized_b->size - 1 - i] + ret;
        if (sum > division->base - 1)
        {
            sum -= division->base;
            ret = 1;
        }
        else
            ret = 0;
        division->normalized_a->value[offset + division->normalized_b->size - i] = sum;
        i += 1;
    }
    if (ret == 1)
        division->normalized_a->value[offset + division->normalized_b->size - i] =
                (division->normalized_a->value[offset + division->normalized_b->size - i] + 1) % division->base;
}

/**
 * Multiplies the dividend by the quotient and subtracts the divider.
 * @param bistromathique: The bistromathique structure.
 * @param division: The division structure.
 * @param offset: The current position in the result.
 * @return: A negative value if an error occurs. Zero if the procedure completes successfully.
 */
int multiply_and_subtract(t_bistromathique bistromathique, t_division *division, int offset)
{
    t_number *subtract = NULL;
    int difference = 0;
    int i = 0;

    if ((subtract = one_digit_multiply(bistromathique, division->normalized_b, division->quotient)) == NULL)
        return -1;
    while (i < division->normalized_b->size + 1)
    {
        difference = division->normalized_a->value[offset + division->normalized_b->size - i] -
                     subtract->value[division->normalized_b->size - i] - division->carry;
        if (difference < 0)
        {
            difference += bistromathique.base_length;
            division->carry = 1;
        }
        else
            division->carry = 0;
        division->normalized_a->value[offset + division->normalized_b->size - i] =
                difference % bistromathique.base_length;
        i += 1;
    }
    if (division->carry == 1)
        division->quotient -= 1;
    free_number(subtract);
    return 0;
}

/**
 * Performs a multiplication on with digit.
 * @param bistromathique: The bistromathique structure.
 * @param number: The number to multiply.
 * @param digit: The digit to multiply.
 * @return: The product of the number and the digit.
 */
t_number *one_digit_multiply(t_bistromathique bistromathique, t_number *number, int digit)
{
    t_number *result = NULL;
    int product = 0;
    int position = 1;

    if ((result = create_number()) == NULL)
        return NULL;
    result->size = number->size + 1;
    if ((result->value = malloc(sizeof(*result) * result->size)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_number(result);
        return NULL;
    }
    while (position <= number->size)
    {
        product += number->value[number->size - position] * digit;
        result->value[result->size - position] = product % bistromathique.base_length;
        product = product / bistromathique.base_length;
        position += 1;
    }
    result->value[result->size - position] = product % bistromathique.base_length;
    return result;
}
