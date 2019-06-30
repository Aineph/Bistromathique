/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the content of the subtraction structure without removing the result.
 * @param subtraction: The subtraction structure.
 */
static void free_subtraction(t_subtraction *subtraction)
{
    if (subtraction->nb_a != NULL)
        free(subtraction->nb_a);
    if (subtraction->nb_b != NULL)
        free(subtraction->nb_b);
    free(subtraction);
}

/**
 * Performs the infinite subtraction.
 * @param subtraction: The subtraction structure.
 */
static void perform_subtraction(t_subtraction *subtraction)
{
    int position = 1;

    subtraction->carry = 0;
    while (position <= subtraction->result->size)
    {
        subtraction->difference = -subtraction->carry;
        if (position <= subtraction->nb_a->size)
            subtraction->difference += subtraction->nb_a->value[subtraction->nb_a->size - position];
        if (position <= subtraction->nb_b->size)
            subtraction->difference -= subtraction->nb_b->value[subtraction->nb_b->size - position];
        subtraction->carry = 0;
        if (subtraction->difference < 0)
        {
            subtraction->difference += subtraction->base;
            subtraction->carry = 1;
        }
        subtraction->result->value[subtraction->result->size - position] = subtraction->difference % subtraction->base;
        position += 1;
    }
    if (remove_leading_zeros(subtraction->result) == -1)
    {
        free_number(subtraction->result);
        subtraction->result = NULL;
    }
}

/**
 * Allocates and initializes the variables of the subtraction structure.
 * @param subtraction: The subtraction structure to initialize.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The initialized subtraction structure.
 */
static t_subtraction *
init_subtraction(t_subtraction *subtraction, t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    subtraction->nb_a = NULL;
    subtraction->nb_b = NULL;
    subtraction->result = NULL;
    if ((subtraction->nb_a = create_number()) == NULL || (subtraction->nb_b = create_number()) == NULL ||
        (subtraction->result = create_number()) == NULL)
    {
        free_number(subtraction->result);
        free_subtraction(subtraction);
        return NULL;
    }
    subtraction->result->sign = SIGN_POS;
    reference_number(subtraction->nb_a, nb_a->value, nb_a->size, nb_a->sign);
    reference_number(subtraction->nb_b, nb_b->value, nb_b->size, nb_b->sign);
    if (nb_a->sign == nb_b->sign && is_higher(nb_b, nb_a))
    {
        subtraction->result->sign = SIGN_NEG;
        reference_number(subtraction->nb_a, nb_b->value, nb_b->size, nb_b->sign);
        reference_number(subtraction->nb_b, nb_a->value, nb_a->size, nb_a->sign);
    }
    subtraction->base = bistromathique.base_length;
    return subtraction;
}

/**
 * Creates the subtraction structure that contains every needed variables.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The subtraction structure.
 */
static t_subtraction *create_subtraction(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_subtraction *subtraction = NULL;

    if ((subtraction = malloc(sizeof(*subtraction))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    if ((subtraction = init_subtraction(subtraction, bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    subtraction->result->size = MAX(nb_a->size, nb_b->size);
    if ((subtraction->result->value = malloc(sizeof(*subtraction->result->value) * subtraction->result->size)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_subtraction(subtraction);
        free_number(subtraction->result);
        return NULL;
    }
    return subtraction;
}

/**
 * Computes the subtraction of two infinite numbers.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The difference of the two given numbers.
 */
t_number *infinite_sub(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_subtraction *subtraction = NULL;
    t_number *result = NULL;

    if ((subtraction = create_subtraction(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    if (nb_a->sign == nb_b->sign)
        perform_subtraction(subtraction);
    else
        free_number(subtraction->result);
    if (nb_a->sign != nb_b->sign && nb_a->sign == SIGN_NEG)
    {
        subtraction->nb_b->sign = SIGN_NEG;
        subtraction->result = infinite_add(bistromathique, subtraction->nb_b, subtraction->nb_a);
        subtraction->result->sign = SIGN_NEG;
    }
    else if (nb_a->sign != nb_b->sign && nb_a->sign == SIGN_POS)
    {
        subtraction->nb_b->sign = SIGN_POS;
        subtraction->result = infinite_add(bistromathique, subtraction->nb_a, subtraction->nb_b);
    }
    result = subtraction->result;
    free_subtraction(subtraction);
    return result;
}
