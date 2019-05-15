/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-15.
 */

#include <stdlib.h>
#include "bistromathique.h"

char *str_copy(const char *source, int length)
{
    char *result = NULL;
    int i = 0;

    if ((result = malloc(sizeof(*result) * (length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < length)
    {
        result[i] = source[i];
        i += 1;
    }
    result[i] = '\0';
    return result;
}

char *str_prepend(char *source, char c, int length)
{
    char *result = NULL;
    int result_length = length + 1;
    int i = 0;

    if ((result = malloc(sizeof(*result) * (result_length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result[0] = c;
    while (i < length)
    {
        result[i + 1] = source[i];
        i += 1;
    }
    result[i + 1] = '\0';
    if (source != NULL)
        free(source);
    return result;
}

char *str_slice(char *source, int index, int length)
{
    char *result = NULL;
    int result_length = (length - index) + 1;
    int i = index;

    if ((result = malloc(sizeof(*result) * (result_length + 1))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < length)
    {
        result[i - index] = source[i];
        i += 1;
    }
    result[i - index] = '\0';
    free(source);
    return result;
}
