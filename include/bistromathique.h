/*
** bistromathique.h for bistromathique in .
** 
** Made by Charlie Root
** Login   <rn@epita.fr>
** 
** Started on  Tue Oct 23 11:48:35 2001 Charlie Root
** Last update Sun Nov  4 13:43:17 2001 Charlie Root
*/

#ifndef _BISTROMATHIQUE_H_
# define _BISTROMATHIQUE_H_

/*
 * Indexes
 */
# define OP_OPEN_PARENT_IDX 0
# define OP_CLOSE_PARENT_IDX 1
# define OP_PLUS_IDX 2
# define OP_SUB_IDX 3
# define OP_NEG_IDX 3
# define OP_MULT_IDX 4
# define OP_DIV_IDX 5
# define OP_MOD_IDX 6

/*
 * Errors
 */

# define SYNTAXE_ERROR_MSG "syntax error\n"
# define READ_ERROR "could not read\n"
# define MALLOC_ERROR "could not alloc\n"
# define OPS_ERROR "Bad ops\n"
# define BASE_ERROR "Bad base\n"

typedef struct s_bistromathique
{
    char *base;
    int base_length;
    char *ops;
    char *expr;
    unsigned int size;
} t_bistromathique;

typedef struct s_number
{
    char *value;
    int size;
} t_number;

typedef t_number (*t_operation)(t_bistromathique, t_number, t_number);

typedef struct s_operation_list
{
    char operator;
    t_operation operation;
    struct s_operation_list *next;
} t_operation_list;

typedef struct s_expression_tree
{
    struct s_expression_tree *first;
    struct s_expression_tree *second;
    char operator;
    t_number result;
} t_expression_tree;

typedef struct s_expression_list
{
    t_expression_tree *expression_root;
    struct s_expression_list *next;
} t_expression_list;

void print_expr(t_expression_tree *);

/*
 * expression.c
 */

void free_expression(t_expression_tree **);

t_expression_tree *parse_left_value(t_bistromathique, int, int);

t_expression_tree *parse_right_value(t_bistromathique, int);

int update_root_expression(t_bistromathique, t_expression_tree **, t_expression_tree *);

t_expression_tree *create_expression(t_bistromathique, int, int);

/*
 * expression_list.c
 */

int pop_expression_from_list(t_expression_list **);

int empty_expression_list(t_expression_list **);

int merge_expressions(t_expression_list **);

int add_expression_to_list(t_expression_list **, t_expression_tree *);

int init_expression_in_list(t_expression_list **);

/*
 * operation_list.c
 */

void free_operation_list(t_operation_list *);

int add_operation_in_list(t_operation_list **, char, t_operation);

t_operation_list *init_operation_list(t_bistromathique);

/*
 * number.c
 */

t_number destroy_number(t_number);

t_number number_to_positive(t_bistromathique, t_number);

t_number number_to_negative(t_bistromathique, t_number);

t_number assign_value_to_number(char *, int);

t_number create_number(void);

/*
 * compute.c
 */

t_number compute(t_bistromathique, t_operation_list *, t_expression_tree *);

/*
 * infinite_add.c
 */

t_number simple_add(t_bistromathique, t_number, t_number);

t_number infinite_add(t_bistromathique, t_number, t_number);

/*
 * infinite_sub.c
 */

t_number simple_sub(t_bistromathique, t_number, t_number);

t_number infinite_sub(t_bistromathique, t_number, t_number);

/*
 * helpers.c
 */

void my_putchar(char);

void my_putstr(char *);

int my_strlen(const char *);

int my_atoi(char *);

int get_value(t_bistromathique, char);

/*
 * compute_helpers.c
 */

int is_negative(t_bistromathique, t_number);

int is_higher(t_bistromathique, t_number, t_number);

char *str_prepend(char *, char);

/*
 * parsing_helpers.c
 */

int is_priority_operator(t_bistromathique, char);

int is_operator(t_bistromathique, char);

/*
 * eval_expr.c
 */

char *eval_expr(char *, char *, char *, unsigned int);

#endif /* !_BISTROMATHIQUE_H_ */