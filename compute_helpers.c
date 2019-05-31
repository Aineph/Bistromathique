/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Checks if a number is negative.
 * @param number: The number to process.
 * @return: A positive value if the given number is negative. Zero if not.
 */
int is_negative(t_number *number)
{
    if (number != NULL && number->sign == SIGN_NEG)
        return 1;
    return 0;
}

/**
 * Checks if a number is equal to zero.
 * @param bistromathique: The bistromathique structure.
 * @param number: The number to process.
 * @return: A positive value if the given number is equal to zero. Zero if not.
 */
int is_null(t_number *number)
{
    int index = 0;

    if (number == NULL)
        return 1;
    if (number->value == NULL)
        return 1;
    while (index < number->size)
    {
        if (number->value[index] != 0)
            return 0;
        index += 1;
    }
    return 1;
}

/**
 * Checks if a number is greater than an other.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number to compare.
 * @param nb_b: The second number to compare.
 * @return: A positive value if the first number is greater than the second. Zero if not.
 */
int is_higher(t_number *nb_a, t_number *nb_b)
{
    int index = 0;

    if ((!is_negative(nb_a) && is_negative(nb_b)) ||
        (is_negative(nb_a) && is_negative(nb_b) && nb_b->size > nb_a->size) ||
        (!is_negative(nb_a) && !is_negative(nb_b) && nb_a->size > nb_b->size))
        return 1;
    else if ((is_negative(nb_a) && !is_negative(nb_b)) ||
             (is_negative(nb_a) && is_negative(nb_b) && nb_b->size < nb_a->size) ||
             (!is_negative(nb_a) && !is_negative(nb_b) && nb_a->size < nb_b->size))
        return 0;
    while (index < nb_a->size)
    {
        if ((is_negative(nb_a) &&
             nb_a->value[index] < nb_b->value[index]) ||
            (!is_negative(nb_a) &&
             nb_a->value[index] > nb_b->value[index]))
            return 1;
        else if ((is_negative(nb_a) &&
                  nb_a->value[index] > nb_b->value[index]) ||
                 (!is_negative(nb_a) &&
                  nb_a->value[index] < nb_b->value[index]))
            return 0;
        index += 1;
    }
    return 0;
}

/**
 * Removes the leading zeros of a number.
 * @param bistromathique: The bistromathique structure.
 * @param result: The number to process.
 * @return: A negative value if an error occurs. Zero if the removal completes successfully.
 */
int epur_result(t_bistromathique bistromathique, t_number *number)
{
    int index = 0;

    while (index < number->size && number->value[index] == 0)
        index += 1;
    if (index > 0 && (number->value = str_slice(number->value, index, number->size)) == NULL)
        return -1;
    number->size -= index;
    if (number->size == 0 && index > 0)
        expr_to_number(bistromathique, number, &bistromathique.base[0], 1);
    return 0;
}
