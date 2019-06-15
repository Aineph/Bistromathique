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
# define ZERO_DIVISION_ERROR "Division by zero\n"

/*
 * Macros
 */

# define MAX(a, b) ((a > b) ? (a) : (b))

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

typedef enum e_sign
{
    SIGN_POS, SIGN_NEG
} t_sign;

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
    t_sign sign;
} t_number;

/**
 * @typedef t_addition
 * The necessary variables needed to perform the addition algorithm.
 * @var nb_a: The first number.
 * @var nb_b: The second number.
 * @var result: The result.
 * @var sum: The temporary sum.
 * @var carry: The temporary carry.
 * @var base: The base of computation.
 */
typedef struct s_addition
{
    t_number *nb_a;
    t_number *nb_b;
    t_number *result;
    int sum;
    int carry;
    int base;
} t_addition;

/**
 * @typedef t_subtraction
 * The necessary variables needed to perform the subtraction algorithm.
 * @var nb_a: The first number.
 * @var nb_b: The second number.
 * @var result: The result.
 * @var difference: The temporary difference.
 * @var carry: The temporary carry.
 * @var base: The base of computation.
 */
typedef struct s_subtraction
{
    t_number *nb_a;
    t_number *nb_b;
    t_number *result;
    int difference;
    int carry;
    int base;
} t_subtraction;

/**
 * @typedef t_multiplication
 * The necessary variables needed to perform the multiplication algorithm.
 * @var middle: The middle offset of the numbers.
 * @var result: The result.
 * @var product: The temporary product.
 * @var carry: The temporary carry.
 * @var base: The base of computation.
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
    t_number *result;
    int product;
    int carry;
    int base;
    t_number *high_a;
    t_number *low_a;
    t_number *high_b;
    t_number *low_b;
    t_number *a1_a;
    t_number *a1_b;
    t_number *a0;
    t_number *a1;
    t_number *a2;
} t_multiplication;

/**
 * @typedef t_division
 * The necessary variables needed to perform the division algorithm.
 * @var normalized_a: The normalized value of the first number.
 * @var normalized_b: The normalized value of the second number.
 * @var result: The result.
 * @var quotient: The temporary quotient.
 * @var carry: The temporary carry.
 * @var base: The base of computation.
 */
typedef struct s_division
{
    t_number *normalized_a;
    t_number *normalized_b;
    t_number *result;
    int quotient;
    int carry;
    int base;
} t_division;

/**
 * @typedef t_operation
 * A function pointer that points to an operation function.
 */
typedef t_number *(*t_operation)(t_bistromathique, t_number *, t_number *);

/**
 * @typedef t_operation_map
 * The map associating every operation character to its function.
 */
typedef struct s_operation_map
{
    char operator;
    t_operation operation;
    struct s_operation_map *next;
} t_operation_map;

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
 * @typedef t_expression_stack
 * A stack of expression trees used to manage parenthesis.
 */
typedef struct s_expression_stack
{
    t_expression_tree *expression_root;
    struct s_expression_stack *next;
} t_expression_stack;

void print_expr(t_expression_tree *);

/*
 * eval_expr.c
 */

char *eval_expr(char *, char *, char *, unsigned int);

/*
 * expression.c
 */

void free_expression(t_expression_tree **);

t_expression_tree *parse_right_value(t_bistromathique, int);

void update_root_expression(t_bistromathique, t_expression_tree **, t_expression_tree *);

t_expression_tree *create_expression(t_bistromathique, int, int);

/*
 * expression_stack.c
 */

int pop_expression_from_stack(t_expression_stack **);

int empty_expression_stack(t_expression_stack **);

int merge_expressions(t_expression_stack **);

int create_sub_expression(t_bistromathique, t_expression_stack **, int, int);

/*
 * operation_list.c
 */

void empty_operation_map(t_operation_map *);

t_operation_map *init_operation_map(t_bistromathique);

/*
 * number.c
 */

void free_number(t_number *);

int copy_number(t_number *, char *, int, t_sign sign);

void reference_number(t_number *, char *, int, t_sign);

int expr_to_number(t_bistromathique, t_number *, char *, int);

t_number *create_number(void);

/*
 * compute.c
 */

t_number *compute(t_bistromathique, t_operation_map *, t_expression_tree **);

/*
 * infinite_add.c
 */

t_number *infinite_add(t_bistromathique, t_number *, t_number *);

/*
 * infinite_sub.c
 */

t_number *infinite_sub(t_bistromathique, t_number *, t_number *);

/*
 * infinite_mul.c
 */

t_number *infinite_mul(t_bistromathique, t_number *, t_number *);

/*
 * infinite_div.c
 */

t_number *infinite_div(t_bistromathique, t_number *, t_number *);

/*
 * infinite_mod.c
 */

t_number *infinite_mod(t_bistromathique, t_number *, t_number *);

/*
 * common.c
 */

void my_putstr(char *);

int my_strlen(const char *);

int my_atoi(char *);

/*
 * common_checks.c
 */

int is_higher(t_number *, t_number *);

int is_null(t_number *);

int is_negative(t_number *);

int is_priority_operator(t_bistromathique, char);

int is_operator(t_bistromathique, char);

/*
 * common_divide.c
 */

void guess_quotient(t_division *, int);

void rollback(t_division *, int);

int multiply_and_subtract(t_bistromathique, t_division *, int);

t_number *one_digit_multiply(t_bistromathique, t_number *, int);

/*
 * common_expression.c
 */

t_expression_tree *expression_has_no_operator(t_bistromathique, int, int);

/*
 * common_multiply.c
 */

void init_multiplication_values(t_multiplication *, t_number *, t_number *);

int process_multiplication(t_bistromathique, t_multiplication *);

/*
 * common_numbers.c
 */

void fill_number_with_zeros(t_number *);

int remove_leading_zeros(t_number *);

char *string_to_number(t_bistromathique, const char *, int);

char *number_to_string(t_bistromathique, const char *, int);

/*
 * common_strings.c
 */

char *str_copy(const char *, int);

char *str_prepend(char *, char, int);

char *str_slice(char *, int, int);

#endif /* !_BISTROMATHIQUE_H_ */
