/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-19.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees the content of the division structure without removing the result.
 * @param division: The division structure.
 */
void free_division(t_division *division)
{
    if (division->normalized_a != NULL)
        free_number(division->normalized_a);
    if (division->normalized_b != NULL)
        free_number(division->normalized_b);
    free(division);
}

/**
 * Performs the infinite division.
 * @param bistromathique: The bistromathique structure.
 * @param division: The division structure.
 * @return: A negative value if an error occurs. Zero if the division completes successfully.
 */
static int perform_division(t_bistromathique bistromathique, t_division *division)
{
    int offset = 0;
    int position = 0;
    int begin = 0;

    while (offset < division->normalized_a->size - division->normalized_b->size)
    {
        division->carry = 0;
        guess_quotient(division, offset);
        if (division->quotient != 0)
            if (multiply_and_subtract(bistromathique, division, offset) == -1)
                return -1;
        if (division->carry == 1)
            rollback(division, offset);
        if (division->quotient != 0)
            begin = 1;
        else if (!begin)
            position += 1;
        if (begin)
            division->result->value[offset - position] = division->quotient % bistromathique.base_length;
        offset += 1;
    }
    division->result->size = offset - position;
    return 0;
}

/**
 * Allocates and initializes the variables of the division structure.
 * @param division: The division structure to initialize.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The initialized multiplication structure.
 */
static t_division *init_division(t_division *division, t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    int normalize_ratio = 0;

    division->normalized_a = NULL;
    division->normalized_b = NULL;
    division->result = NULL;
    normalize_ratio = bistromathique.base_length / (nb_b->value[0] + 1);
    division->normalized_a = one_digit_multiply(bistromathique, nb_a, normalize_ratio);
    division->normalized_b = one_digit_multiply(bistromathique, nb_b, normalize_ratio);
    if ((division->normalized_a->value = str_prepend(division->normalized_a->value, 0, division->normalized_a->size)) ==
        NULL)
    {
        free_division(division);
        return NULL;
    }
    division->normalized_a->size += 1;
    if (remove_leading_zeros(division->normalized_b) == -1)
    {
        free_division(division);
        return NULL;
    }
    if ((division->result = create_number()) == NULL)
    {
        free_division(division);
        return NULL;
    }
    return division;
}

/**
 * Creates the division structure that contains every needed variables.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The division structure.
 */
static t_division *create_division(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_division *division = NULL;

    if ((division = malloc(sizeof(*division))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    if ((division = init_division(division, bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    division->result->size = MAX(nb_a->size - nb_b->size + 1, 1);
    if ((division->result->value = malloc(sizeof(*division->result->value) * division->result->size)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        free_number(division->result);
        free_division(division);
        return NULL;
    }
    division->base = bistromathique.base_length;
    return division;
}

/**
 * Computes the division of two infinite numbers.
 * @param bistromathique: The bistromathique structure.
 * @param nb_a: The first number.
 * @param nb_b: The second number.
 * @return: The quotient of the two given numbers.
 */
t_number *infinite_div(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;
    t_division *division = NULL;

    if (is_null(nb_b))
    {
        my_putstr(ZERO_DIVISION_ERROR);
        return NULL;
    }
    if ((division = create_division(bistromathique, nb_a, nb_b)) == NULL)
        return NULL;
    if (is_null(nb_a) || is_higher(nb_b, nb_a))
        division->result->value[0] = 0;
    else
    {
        if (perform_division(bistromathique, division) == -1)
        {
            free_number(division->result);
            free_division(division);
            return NULL;
        }
        if (nb_a->sign != nb_b->sign)
            division->result->sign = SIGN_NEG;
    }
    result = division->result;
    free_division(division);
    return result;
}
