/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

void free_number(t_number *number)
{
    if (number->value != NULL)
        free(number->value);
    free(number);
}

int number_to_positive(t_bistromathique bistromathique, t_number *number)
{
    int number_size = number->size;

    if (number->value != NULL && is_negative(bistromathique, number))
    {
        number->size = 0;
        if ((number->value = str_slice(number->value, 1, number_size)) == NULL)
            return -1;
    }
    number->size = number_size - 1;
    return 0;
}

int number_to_negative(t_bistromathique bistromathique, t_number *number)
{
    int number_size = number->size;

    if (number->value != NULL && !is_negative(bistromathique, number))
    {
        number->size = 0;
        if ((number->value = str_prepend(number->value, '-', number_size)) == NULL)
            return -1;
    }
    number->size = number_size + 1;
    return 0;
}

int assign_value_to_number(t_number *number, char *value, int size)
{
    if (number->value != NULL)
        free(number->value);
    number->size = 0;
    if ((number->value = str_copy(value, size)) == NULL)
        return -1;
    number->size = size;
    return 0;
}

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
    return number;
}