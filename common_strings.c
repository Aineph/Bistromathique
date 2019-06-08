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
