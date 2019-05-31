/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Frees a number.
 * @param number: The number to free.
 */
void free_number(t_number *number)
{
    if (number->value != NULL)
        free(number->value);
    free(number);
}

/**
 * Turns a negative number to positive one.
 * @param number: The number to process.
 */
void number_to_positive(t_number *number)
{
    number->sign = SIGN_POS;
}

/**
 * Turns a positive number to a negative one.
 * @param number: The number to process.
 */
void number_to_negative(t_number *number)
{
    number->sign = SIGN_NEG;
}

/**
 * Assigns a value to a given number.
 * @param number: The number to process.
 * @param value: The value to assign to the given number.
 * @param size: The size to assign to the given number.
 * @param sign: The sign to assign to the given number.
 * @return: A negative value if an error occurs. Zero if the assignment completes successfully.
 */
int assign_value_to_number(t_number *number, char *value, int size, t_sign sign)
{
    if (number->value != NULL)
        free(number->value);
    number->size = 0;
    if ((number->value = str_copy(value, size)) == NULL)
        return -1;
    number->size = size;
    number->sign = sign;
    return 0;
}

/**
 * Creates an empty number with default values.
 * @return: The number created.
 */
t_number *create_number(void)
{
    t_number *number = NULL;

    if ((number = malloc(sizeof(*number))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    number->value = NULL;
    number->size = 0;
    number->sign = SIGN_POS;
    return number;
}
