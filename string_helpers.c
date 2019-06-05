/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-15.
 */

#include <stdlib.h>
#include "bistromathique.h"

/**
 * Copies the content of a string into an other one.
 * @param source: The string to copy.
 * @param length: The length of the string to copy.
 * @return: A copy of the given string with the given length.
 */
char *str_copy(const char *source, int length)
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
        result[index] = source[index];
        index += 1;
    }
    result[index] = '\0';
    return result;
}

/**
 * Adds an extra character at the beginning of a string.
 * @param source: The string to process.
 * @param c: The character to add at the beginning of the string.
 * @param length: The length of the string.
 * @return: The given string with an extra character at its beginning.
 */
char *str_prepend(char *source, char c, int length)
{
    char *result = NULL;
    int result_length = length + 1;
    int index = 0;

    if ((result = malloc(sizeof(*result) * (result_length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result[0] = c;
    while (index < length)
    {
        result[index + 1] = source[index];
        index += 1;
    }
    result[index + 1] = '\0';
    if (source != NULL)
        free(source);
    return result;
}

/**
 * Truncates a given string at a given index.
 * @param source: The string to process.
 * @param index: The start index of the string.
 * @param length: The length of the string truncated.
 * @return: The truncated string.
 */
char *str_slice(char *source, int index, int length)
{
    char *result = NULL;
    int result_length = (length - index) + 1;
    int position = index;

    if ((result = malloc(sizeof(*result) * (result_length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (position < length)
    {
        result[position - index] = source[position];
        position += 1;
    }
    result[position - index] = '\0';
    free(source);
    return result;
}

/**
 * Adds one or more characters at the end of a string.
 * @param str: The string to process.
 * @param length: The length of the string.
 * @param c: The character to add at the end of the string.
 * @param count: The number of times to add the given character.
 * @return: The right-padded string.
 */
char *str_rpad(char *str, int length, char c, int count)
{
    char *result = NULL;
    int i = 0;
    int j = 0;

    if ((result = malloc(sizeof(*result) * (length + count))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (str != NULL && i < length)
    {
        result[i] = str[i];
        i += 1;
    }
    while (j < count)
    {
        result[i + j] = c;
        j += 1;
    }
    if (str != NULL)
        free(str);
    return result;
}

/**
 * Change the content of a string to its numerical value in a char array.
 * @param bistromathique: The bistromathique structure.
 * @param source: The string to process.
 * @param length: The length of the string to process.
 * @return: A char array containing numerical values.
 */
char *str_to_val(t_bistromathique bistromathique, const char *source, int length)
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
char *val_to_str(t_bistromathique bistromathique, char *source, int length)
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
