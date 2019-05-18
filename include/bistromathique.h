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

/*
 * Macros
 */

# define MIN(a, b) ((a < b) ? (a) : (b))

/**
 * @typedef t_bistromathique
 * The main structure of the project containing the parameters.
 * @var base: The base of the numbers to compute.
 * @var base_length: The length of the base.
 * @var ops: The string of the operators allowed in the expression.
 * @var expr: The expression to compute.
 */
typedef struct s_bistromathique
{
    char *base;
    int base_length;
    char *ops;
    char *expr;
    unsigned int size;
} t_bistromathique;

/**
 * @typedef t_number
 * The representation of an infinite number.
 * @var value: The string representation of the number.
 * @var size: The length of the number.
 */
typedef struct s_number
{
    char *value;
    int size;
} t_number;

/**
 * @typedef t_multiplication
 * The necessary variables needed to perform the multiplication algorithm.
 * @var middle: The middle offset of the numbers.
 * @var high_a: The high part of number A.
 * @var low_a: The low part of number A.
 * @var high_b: The high part of number B.
 * @var low_b: The low part of number B.
 * @var a0: The result of the expression high_a * high_b.
 * @var a1: The result of the expression (high_a + low_a) * (high_b + low_b).
 * @var a2: The result of the expression low_a * low_b.
 */
typedef struct s_multiplication
{
    int middle;
    t_number *high_a;
    t_number *low_a;
    t_number *high_b;
    t_number *low_b;
    t_number *a0;
    t_number *a1;
    t_number *a2;
} t_multiplication;

/**
 * @typedef t_operation
 * A function pointer that points to an operation function.
 */
typedef t_number *(*t_operation)(t_bistromathique, t_number *, t_number *);

/**
 * @typedef t_operation_list
 * The list of every operations.
 */
typedef struct s_operation_list
{
    char operator;
    t_operation operation;
    struct s_operation_list *next;
} t_operation_list;

/**
 * @typedef t_expression_tree
 * The tree representing the expression as a binary tree.
 */
typedef struct s_expression_tree
{
    struct s_expression_tree *first;
    struct s_expression_tree *second;
    char operator;
    int level;
    t_number *result;
} t_expression_tree;

/**
 * @typedef t_expression_list
 * A list of several expression.
 */
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

t_expression_tree *create_expression(t_bistromathique, int, int, int);

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

void empty_operation_list(t_operation_list *);

int add_operation_in_list(t_operation_list **, char, t_operation);

t_operation_list *init_operation_list(t_bistromathique);

/*
 * number.c
 */

void free_number(t_number *);

int number_to_positive(t_bistromathique, t_number *);

int number_to_negative(t_bistromathique, t_number *);

int assign_value_to_number(t_number *, char *, int);

t_number *create_number(void);

/*
 * compute.c
 */

t_number *compute(t_bistromathique, t_operation_list *, t_expression_tree **);

/*
 * infinite_add.c
 */

t_number *simple_add(t_bistromathique, t_number *, t_number *);

t_number *infinite_add(t_bistromathique, t_number *, t_number *);

/*
 * infinite_sub.c
 */

t_number *simple_sub(t_bistromathique, t_number *, t_number *);

t_number *infinite_sub(t_bistromathique, t_number *, t_number *);

/*
 * infinite_mul.c
 */

t_number *simple_mul(t_bistromathique, t_number *, t_number *);

t_number *infinite_mul(t_bistromathique, t_number *, t_number *);

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

int is_negative(t_bistromathique, t_number *);

int is_higher(t_bistromathique, t_number *, t_number *);

int epur_result(t_bistromathique, t_number *);

/*
 * string_helpers.c
 */

char *str_copy(const char *, int);

char *str_prepend(char *, char, int);

char *str_slice(char *, int, int);

char *str_rpad(char *, int, char, int);

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