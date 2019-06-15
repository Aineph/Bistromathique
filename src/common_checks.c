/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Checks if a number is greater than an other.
 * @param nb_a: The first number to compare.
 * @param nb_b: The second number to compare.
 * @return: A positive value if the first number is greater than the second. Zero if not.
 */
int is_higher(t_number *nb_a, t_number *nb_b)
{
    int index = 0;

    if (nb_a->size > nb_b->size)
        return 1;
    else if (nb_a->size < nb_b->size)
        return 0;
    while (index < nb_a->size)
    {
        if (nb_a->value[index] > nb_b->value[index])
            return 1;
        else if (nb_a->value[index] < nb_b->value[index])
            return 0;
        index += 1;
    }
    return 0;
}

/**
 * Checks if a number is equal to zero.
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
 * Checks if a character is a priority operator.
 * @param bistromathique: The bistromathique structure.
 * @param c: The character to process.
 * @return: A positive value if the given character is a priority operator. Zero if not.
 */
int is_priority_operator(t_bistromathique bistromathique, char c)
{
    int index = 0;

    while (bistromathique.ops[index] != '\0')
    {
        if (c == bistromathique.ops[index] && (index == OP_MULT_IDX || index == OP_DIV_IDX || index == OP_MOD_IDX))
            return 1;
        index += 1;
    }
    return 0;
}

/**
 * Checks if a character is an operator.
 * @param bistromathique: The bistromathique structure.
 * @param c: The character to process.
 * @return: A positive value if the given character is an operator. Zero if not.
 */
int is_operator(t_bistromathique bistromathique, char c)
{
    int index = 0;

    while (bistromathique.ops[index] != '\0')
    {
        if (c == bistromathique.ops[index])
            return 1;
        index += 1;
    }
    return 0;
}
