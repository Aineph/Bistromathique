/*
** main.c for bistromathique 
** 
** Made by Charlie Root
** Login   <rn@epita.fr>
** 
** Started on  Tue Oct 23 11:45:05 2001 Charlie Root
** Last update Mon Sep 17 12:00:27 2012 Mickael Wiart
*/

#include <unistd.h>
#include <stdlib.h>
#include "bistromathique.h"

/**
 * Checks if the base entered by the user is correct.
 * @param base: The list of the base digits represented as a string.
 */
static void check_base(char *base)
{
    if (my_strlen(base) < 2)
    {
        my_putstr(BASE_ERROR);
        exit(1);
    }
}

/**
 * Checks if the operators entered by the user are correct.
 * @param ops: The list of operators represented as a string.
 */
static void check_ops(char *ops)
{
    if (my_strlen(ops) != 7)
    {
        my_putstr(OPS_ERROR);
        exit(5);
    }
}

/**
 * Allocates the memory to store the expression.
 * @param size: The size of the expression.
 * @return expr: The arithmetic expression to compute.
 */
static char *get_expr(unsigned int size)
{
    char *expr;
    ssize_t read_size = 0;
    ssize_t tmp_size = 0;

    if (size <= 0)
    {
        my_putstr("Bad expr len\n");
        exit(2);
    }
    if ((expr = malloc(size + 1)) == NULL)
    {
        my_putstr(MALLOC_ERROR);
        exit(3);
    }
    while ((read_size = read(0, &expr[tmp_size], size)) > 0)
        tmp_size += read_size;
    if (tmp_size < size)
    {
        my_putstr(READ_ERROR);
        exit(4);
    }
    expr[size] = '\0';
    return (expr);
}

/**
 * The main function.
 * @param ac: The number of arguments passed as parameter.
 * @param av: The arguments values.
 * @return: Zero if everything works fine. An other value if a problem occurs.
 */
int main(int ac, char **av)
{
    char *expr;
    unsigned int size;

    if (ac != 4)
    {
        my_putstr("Usage : ");
        my_putstr(av[0]);
        my_putstr(" base ops\"()+-*/%\" exp_len\n");
        exit(1);
    }
    check_base(av[1]);
    check_ops(av[2]);
    size = (unsigned int) my_atoi(av[3]);
    expr = get_expr(size);
    my_putstr(eval_expr(av[1], av[2], expr, size));
    free(expr);
    return 0;
}
