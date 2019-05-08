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
# define SYNTAXE_ERROR_MSG "syntax error\n"

/*
 * Errors
 */

# define READ_ERROR "could not read\n"
# define MALLOC_ERROR "could not alloc\n"
# define OPS_ERROR "Bad ops\n"
# define BASE_ERROR "Bad base\n"

typedef struct s_bistromathique
{
    char *base;
    char *ops;
    char *expr;
    unsigned int size;
} t_bistromathique;

typedef struct s_operation
{
    char operator;

    char *(*operation)(t_bistromathique, char *, char *);

    struct s_operation *next;
} t_operation;

typedef struct s_expression
{
    struct s_expression *first;
    struct s_expression *second;
    char operator;
    char *result;
} t_expression;

typedef struct s_expression_list
{
    t_expression *expression;
    struct s_expression_list *next;
} t_expression_list;

void print_expr(t_expression *);

/*
 * expression.c
 */

void free_expression(t_expression **);

t_expression *parse_left_value(t_bistromathique, int);

t_expression *parse_right_value(t_bistromathique, int);

int update_root_expression(t_bistromathique, t_expression **, t_expression *);

t_expression *create_expression(t_bistromathique, int);

/*
 * expression_list.c
 */

int pop_expression_from_list(t_expression_list **);

int empty_expression_list(t_expression_list **);

int merge_expressions(t_expression_list **);

int add_expression_to_list(t_expression_list **, t_expression *);

int init_expression_in_list(t_expression_list **);

/*
 * operations.c
 */

void free_operations(t_operation *);

int add_operation(t_operation **, char, char *(*)(t_bistromathique, char *, char *));

t_operation *init_operations(t_bistromathique);

/*
 * compute.c
 */

char *compute(t_bistromathique, t_operation *, t_expression *);

/*
 * infinite_add.c
 */

char *infinite_add(t_bistromathique, char *, char *);

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

int is_negative(t_bistromathique, const char *);

int is_higher(t_bistromathique, char *, char *);

/*
 * parsing_helpers.c
 */

char *concat_before(char *, char);

char *concat_after(char *, char);

int is_priority_operator(char *, char);

int is_operator(char *, char);

/*
 * eval_expr.c
 */

char *eval_expr(char *, char *, char *, unsigned int);

#endif /* !_BISTROMATHIQUE_H_ */