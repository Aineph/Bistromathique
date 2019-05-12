/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

t_number_list *
perform_multiplication(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b, t_number_list *number_list)
{
    int offset_a;
    int offset_b = nb_b->size - 1;
    int tmp_result = 0;
    char ret;
    int level = 0;
    int level_save = 0;
    t_number *new_number = NULL;

    while (offset_b >= 0)
    {
        if ((new_number = create_number()) == NULL)
            return NULL;
        offset_a = nb_a->size - 1;
        ret = bistromathique.base[0];
        new_number->size = nb_a->size + level;
        if ((new_number->value = malloc(sizeof(*new_number->value) * (new_number->size + 1))) == NULL)
        {
            my_putstr(MALLOC_ERROR);
            empty_number_list(number_list);
            return NULL;
        }
        level_save = level;
        new_number->value[new_number->size] = '\0';
        while (level > 0)
            new_number->value[nb_a->size + --level] = bistromathique.base[0];
        while (offset_a >= 0)
        {
            tmp_result = (get_value(bistromathique, nb_b->value[offset_b]) *
                          get_value(bistromathique, nb_a->value[offset_a])) + get_value(bistromathique, ret);
            ret = bistromathique.base[tmp_result / bistromathique.base_length];
            new_number->value[offset_a] = bistromathique.base[tmp_result % bistromathique.base_length];
            offset_a -= 1;
        }
        if (ret != bistromathique.base[0])
        {
            if ((new_number->value = str_prepend(new_number->value, ret, new_number->size)) == NULL)
                return NULL;
            new_number->size += 1;
        }
        if (add_number_to_list(&number_list, new_number) == -1)
        {
            empty_number_list(number_list);
            free_number(new_number);
            return NULL;
        }
        offset_b -= 1;
        level = level_save + 1;
    }
    return number_list;
}

t_number *simple_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result = NULL;
    t_number *result_save;
    t_number_list *number_list = NULL;
    t_number_list *number_save = NULL;

    if (nb_a->value == NULL || nb_b->value == NULL)
        return NULL;
    if ((number_list = perform_multiplication(bistromathique, nb_a, nb_b, number_list)) == NULL)
        return NULL;
    if ((result = create_number()) == NULL)
        return NULL;
    while (number_list != NULL)
    {
        number_save = number_list->next;
        result_save = simple_add(bistromathique, result, number_list->number);
        free_number(number_list->number);
        free_number(result);
        if ((result = create_number()) == NULL)
            return NULL;
        result->value = result_save->value;
        result->size = result_save->size;
        free(number_list);
        number_list = number_save;
    }
    return result;
}

t_number *infinite_mul(t_bistromathique bistromathique, t_number *nb_a, t_number *nb_b)
{
    t_number *result;

    if (is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_a) == -1)
            return NULL;
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
    }
    else if (is_negative(bistromathique, nb_a) && !is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_a) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
    }
    else if (!is_negative(bistromathique, nb_a) && is_negative(bistromathique, nb_b))
    {
        if (number_to_positive(bistromathique, nb_b) == -1)
            return NULL;
        result = simple_mul(bistromathique, nb_a, nb_b);
        if (number_to_negative(bistromathique, result) == -1)
            return NULL;
    }
    else
        result = simple_mul(bistromathique, nb_a, nb_b);
    return result;
}