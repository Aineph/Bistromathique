/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number destroy_number(t_number number)
{
    if (number.value != NULL)
        free(number.value);
    number.value = NULL;
    return number;
}

t_number number_to_positive(t_bistromathique bistromathique, t_number number)
{
    t_number positive_number = create_number();
    int index = 1;

    if (!is_negative(bistromathique, number) || number.value == NULL)
        return number;
    if ((positive_number.value = malloc(sizeof(*positive_number.value) * (positive_number.size + 1))) == NULL)
        return positive_number;
    positive_number.size = number.size - 1;
    while (index < number.size)
    {
        positive_number.value[index - 1] = number.value[index];
        index += 1;
    }
    positive_number.value[index - 1] = '\0';
    free(number.value);
    return positive_number;
}

t_number number_to_negative(t_bistromathique bistromathique, t_number number)
{
    t_number negative_number = create_number();

    if (is_negative(bistromathique, number) || number.value == NULL)
        return number;
    if ((negative_number.value = str_prepend(number.value, '-')) == NULL)
        return negative_number;
    negative_number.size = number.size + 1;
    return negative_number;
}

t_number assign_value_to_number(char *value, int size)
{
    t_number number;
    int index = 0;

    number.size = 0;
    if (value != NULL)
    {
        if ((number.value = malloc(sizeof(*number.value) * (size + 1))) == NULL)
            number.value = NULL;
        else
        {
            while (index < size)
            {
                number.value[index] = value[index];
                index += 1;
            }
            number.value[index] = '\0';
            number.size = size;
        }
    }
    return number;
}

t_number create_number(void)
{
    t_number number;

    number.value = NULL;
    number.size = 0;
    return number;
}