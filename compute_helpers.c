/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_negative(t_bistromathique bistromathique, const char *number)
{
    if (number[0] == bistromathique.ops[OP_NEG_IDX])
        return 1;
    return 0;
}

int is_higher(t_bistromathique bistromathique, char *nb_a, char *nb_b)
{
    int len1 = my_strlen(nb_a);
    int len2 = my_strlen(nb_b);
    int i = 0;

    if ((!is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
        (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && len2 > len1) ||
        (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && len1 > len2))
        return 1;
    else if ((is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
             (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && len2 < len1) ||
             (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && len1 < len2))
        return 0;
    while (i < len1)
    {
        if ((is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a[i]) < get_value(bistromathique, nb_b[i])) ||
            (!is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a[i]) > get_value(bistromathique, nb_b[i])))
            return 1;
        else if ((is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a[i]) > get_value(bistromathique, nb_b[i])) ||
                 (!is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a[i]) < get_value(bistromathique, nb_b[i])))
            return 0;
        i += 1;
    }
    return 0;
}

char *str_prepend(char *source, char c)
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