/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

#include <stdlib.h>
#include "bistromathique.h"

int is_negative(t_bistromathique bistromathique, t_number number)
{
    if (number.value[0] == bistromathique.ops[OP_NEG_IDX])
        return 1;
    return 0;
}

int is_higher(t_bistromathique bistromathique, t_number nb_a, t_number nb_b)
{
    int i = 0;

    if ((!is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
        (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && nb_b.size > nb_a.size) ||
        (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && nb_a.size > nb_b.size))
        return 1;
    else if ((is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b)) ||
             (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b) && nb_b.size < nb_a.size) ||
             (!is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b) && nb_a.size < nb_b.size))
        return 0;
    while (i < nb_a.size)
    {
        if ((is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a.value[i]) < get_value(bistromathique, nb_b.value[i])) ||
            (!is_negative(bistromathique, nb_a) &&
             get_value(bistromathique, nb_a.value[i]) > get_value(bistromathique, nb_b.value[i])))
            return 1;
        else if ((is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a.value[i]) > get_value(bistromathique, nb_b.value[i])) ||
                 (!is_negative(bistromathique, nb_a) &&
                  get_value(bistromathique, nb_a.value[i]) < get_value(bistromathique, nb_b.value[i])))
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