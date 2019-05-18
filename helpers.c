/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <unistd.h>
#include "bistromathique.h"

void my_putstr(char *str)
{
    if (str != NULL)
        write(1, str, my_strlen(str));
}

int my_strlen(const char *str)
{
    int i = 0;

    while (str != NULL && str[i] != '\0')
        i += 1;
    return i;
}

int my_atoi(char *value)
{
    int result = 0;
    int is_neg = 0;
    int multiplier = 1;
    int offset = my_strlen(value) - 1;

    while (offset >= 0 && value[offset] >= '0' && value[offset] <= '9')
    {
        result += (value[offset] - '0') * multiplier;
        multiplier *= 10;
        offset -= 1;
    }
    while (offset >= 0)
    {
        if (value[offset] == '-')
            is_neg = !is_neg;
        else if (value[offset] != '+')
            result = 0;
        offset -= 1;
    }
    if (is_neg)
        result = -result;
    return result;
}

int get_value(t_bistromathique bistromathique, char value)
{
    int i = 0;

    while (bistromathique.base[i] != '\0')
    {
        if (bistromathique.base[i] == value)
            return i;
        i += 1;
    }
    return -1;
}