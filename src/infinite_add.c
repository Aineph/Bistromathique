/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 21/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the content of the addition structure without removing the result.
 * @param addition: The addition structure.
 */
static void free_addition(t_addition *addition)
{
    if (addition->nb_a != NULL)
        free(addition->nb_a);
    if (addition->nb_b != NULL)
        free(addition->nb_b);
    free(addition);
}

/**
 * Performs the infinite addition.
 * @param addition: The addition structure.
 */
static void perform_addition(t_addition *addition)
{
    int position = 1;

    addition->carry = 0;
    while (position <= addition->result->size)
    {
        addition->sum = 0;
        if (position <= addition->nb_a->size)
            addition->sum += addition->nb_a->value[addition->nb_a->size - position];
        if (position <= addition->nb_b->size)
            addition->sum += addition->nb_b->value[addition->nb_b->size - position];
        addition->sum += addition->carry;
        addition->result->value[addition->result->size - position] = addition->sum % addition->base;
        addition->carry = addition->sum / addition->base;
        position += 1;
    }
    if (remove_leading_zeros(addition->result) == -1)
    {
        free_number(addition->result);
        addition->result = NULL;
    }
}

/**
 * Allocates and initializes the variables of the addition structure.
 * @param addition: The addition structure to initialize.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The initialized addition structure.
 */
static t_addition *init_addition(t_addition *addition, t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    addition->nb_a = NULL;
    addition->nb_b = NULL;
    addition->result = NULL;
    if ((addition->nb_a = create_number()) == NULL || (addition->nb_b = create_number()) == NULL ||
        (addition->result = create_number()) == NULL)
    {
        free_number(addition->result);
        free_addition(addition);
        return NULL;
    }
    reference_number(addition->nb_a, nb_a->value, nb_a->size, nb_a->sign);
    reference_number(addition->nb_b, nb_b->value, nb_b->size, nb_b->sign);
    addition->base = bistromathique.base_length;
    return addition;
}

/**
 * Creates the addition structure that contains every needed variables.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The addition structure.
 */
static t_addition *create_addition(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_addition *addition = NULL;

    if ((addition = malloc(sizeof(*addition))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    if ((addition = init_addition(addition, bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    addition->result->size = MAX(nb_a->size, nb_b->size) + 1;
    if ((addition->result->value = malloc(sizeof(*addition->result->value) * addition->result->size)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_addition(addition);
        free_number(addition->result);
        return NULL;
    }
    return addition;
}

/**
 * Computes the addition of two infinite numbers.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The sum of the two given numbers.
 */
t_number *infinite_add(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_addition *addition = NULL;
    t_number *result = NULL;

    if ((addition = create_addition(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    if (nb_a->sign == nb_b->sign)
        perform_addition(addition);
    else
        free_number(addition->result);
    if (nb_a->sign != nb_b->sign && nb_a->sign == SIGN_NEG)
    {
        addition->nb_a->sign = SIGN_POS;
        addition->result = infinite_sub(bistromathique, addition->nb_b, addition->nb_a);
    }
    else if (nb_a->sign != nb_b->sign && nb_b->sign == SIGN_NEG)
    {
        addition->nb_b->sign = SIGN_POS;
        addition->result = infinite_sub(bistromathique, addition->nb_a, addition->nb_b);
    }
    result = addition->result;
    free_addition(addition);
    return result;
}
