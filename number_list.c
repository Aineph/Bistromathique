/*
 * Created by Nicolas Fez for Bistromathique.
 * Started on 2019-05-11.
 */

#include <stdlib.h>
#include "bistromathique.h"

void empty_number_list(t_number_list *number_list)
{
    t_number_list *number_tmp = number_list;
    t_number_list *number_save;

    while (number_tmp != NULL)
    {
        number_save = number_tmp->next;
        destroy_number(number_tmp->number);
        free(number_tmp);
        number_tmp = number_save;
    }
}

int add_number_to_list(t_number_list **list, t_number number)
{
    t_number_list *new_element;

    if ((new_element = malloc(sizeof(*new_element))) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        return -1;
    }
    new_element->number = number;
    new_element->next = *list;
    *list = new_element;
    return 0;
}