/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 08/05/2019.
 */

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