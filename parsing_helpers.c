/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 20/04/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

char *concat_before(char *source, char c)
{
    char *result = NULL;
    int source_length = my_strlen(source);
    int result_length = source_length + 1;
    int i = 0;

    if ((result = malloc(sizeof(*result) * result_length + 1)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    result[0] = c;
    while (i < source_length)
    {
        result[i + 1] = source[i];
        i += 1;
    }
    result[i + 1] = '\0';
    if (source != NULL)
        free(source);
    return result;
}

char *concat_after(char *source, char c)
{
    char *result = NULL;
    int source_length = my_strlen(source);
    int result_length = source_length + 1;
    int i = 0;

    if ((result = malloc(sizeof(*result) * result_length + 1)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return NULL;
    }
    while (i < source_length)
    {
        result[i] = source[i];
        i += 1;
    }
    result[i] = c;
    result[i + 1] = '\0';
    if (source != NULL)
        free(source);
    return result;
}

int is_priority_operator(char *operators, char c)
{
    int i = 0;

    while (operators[i] != '\0')
    {
        if (c == operators[i] && (i == OP_MULT_IDX || i == OP_DIV_IDX || i == OP_MOD_IDX))
            return 1;
        i += 1;
    }
    return 0;
}

int is_operator(char *operators, char c)
{
    int i = 0;

    while (operators[i] != '\0')
    {
        if (c == operators[i])
            return 1;
        i += 1;
    }
    return 0;
}