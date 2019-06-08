/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Retrieves the decimal value of a character in the base of bistromathique.
 * @param bistromathique: The bistromathique structure.
 * @param value: The character to process.
 * @return: The decimal value corresponding to the character in the base of bistromathique.
 */
static int get_value(t_bistromathique bistromathique, char value)
{
    int index = 0;

    while (bistromathique.base[index] != '\0')
    {
        if (bistromathique.base[index] == value)
            return index;
        index += 1;
    }
    return -1;
}

/**
 * Sets the content of a given number to zero.
 * @param number: The number to process.
 */
void fill_number_with_zeros(t_number *number)
{
    int index = 0;

    while (index < number->size)
    {
        number->value[index] = 0;
        index += 1;
    }
}

/**
 * Removes the leading zeros of a number.
 * @param number: The number to process.
 * @return: A negative value if an error occurs. Zero if the removal completes successfully.
 */
int remove_leading_zeros(t_number *number)
{
    int index = 0;
    char zero = 0;

    while (index < number->size && number->value[index] == 0)
        index += 1;
    if (number->size - index == 0 && index > 0)
    {
        if (copy_number(number, &zero, 1, SIGN_POS) == -1)
            return -1;
        return 0;
    }
    if (index > 0 && (number->value = str_slice(number->value, index, number->size)) == NULL)
        return -1;
    number->size -= index;
    return 0;
}

/**
 * Change the content of a string to its numerical value in a char array.
 * @param bistromathique: The bistromathique structure.
 * @param source: The string to process.
 * @param length: The length of the string to process.
 * @return: A char array containing numerical values.
 */
char *string_to_number(t_bistromathique bistromathique, const char *source, int length)
{
    char *result = NULL;
    int index = 0;

    if ((result = malloc(sizeof(*result) * length)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (index < length)
    {
        if ((result[index] = get_value(bistromathique, source[index])) == -1)
        {
            my_putstr(SYNTAXE_ERROR_MSG);
            free(result);
            return NULL;
        }
        index += 1;
    }
    return result;
}

/**
 * Change the content of a char array to its printable value in a string.
 * @param bistromathique: The bistromathique structure.
 * @param source: The string to process.
 * @param length: The length of the string to process.
 * @return: A string containing printable values.
 */
char *number_to_string(t_bistromathique bistromathique, const char *source, int length)
{
    char *result = NULL;
    int index = 0;

    if ((result = malloc(sizeof(*result) * (length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (index < length)
    {
        result[index] = bistromathique.base[source[index]];
        index += 1;
    }
    result[index] = '\0';
    return result;
}
