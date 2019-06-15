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
    if (number != NULL)
    {
        if (number->value != NULL)
            free(number->value);
        free(number);
    }
}

/**
 * Copies contents in a given number.
 * @param number: The number to process.
 * @param value: The value to assign to the given number.
 * @param size: The size to assign to the given number.
 * @param sign: The sign to assign to the given number.
 * @return: A negative value if an error occurs. Zero if the assignment completes successfully.
 */
int copy_number(t_number *number, char *value, int size, t_sign sign)
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
 * Copies contents in a given number without allocating it. Also remove every leading zeros.
 * @param number: The number to process.
 * @param value: The value to assign to the given number.
 * @param size: The size to assign to the given number.
 * @param sign: The sign to assign to the given number.
 */
void reference_number(t_number *number, char *value, int size, t_sign sign)
{
    int index = 0;

    number->size = size;
    if (value == NULL)
        number->value = NULL;
    else
    {
        while (index < size && value[index] == 0)
            index += 1;
        number->size -= index;
        number->value = &value[index];
    }
    number->sign = sign;
}

/**
 * Converts a printable string to a number.
 * @param bistromathique: The bistromathique structure.
 * @param number: The number to process.
 * @param value: The value to convert and assign to the given number.
 * @param size: The size to assign to the given number.
 * @return: A negative value if an error occurs. Zero if the assignment completes successfully.
 */
int expr_to_number(t_bistromathique bistromathique, t_number *number, char *value, int size)
{
    if (number->value != NULL)
        free(number->value);
    number->size = 0;
    if ((number->value = string_to_number(bistromathique, value, size)) == NULL)
        return -1;
    number->size = size;
    number->sign = SIGN_POS;
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
