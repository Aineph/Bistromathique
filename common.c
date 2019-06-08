/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 17/04/2019.
 */

#include <unistd.h>
#include "bistromathique.h"

/**
 * Prints a string on the standard output.
 * @param str: The string to print.
 */
void my_putstr(char *str)
{
    if (str != NULL)
        write(1, str, my_strlen(str));
}

/**
 * Computes the length of a string.
 * @param str: The string to process.
 * @return: The length of the given string.
 */
int my_strlen(const char *str)
{
    int index = 0;

    while (str != NULL && str[index] != '\0')
        index += 1;
    return index;
}

/**
 * Transforms a string into an integer.
 * @param value: The string to turn into an integer.
 * @return: The integer corresponding to the given string.
 */
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
