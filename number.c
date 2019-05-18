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

void number_to_positive(t_number *number)
{
    number->sign = SIGN_POS;
}

void number_to_negative(t_number *number)
{
    number->sign = SIGN_NEG;
}

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