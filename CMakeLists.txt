cmake_minimum_required(VERSION 3.12)
project(Bistromathique C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(Bistromathique main.c common.c eval_expr.c compute.c expression.c common_checks.c expression_stack.c operation_map.c infinite_add.c common_numbers.c infinite_sub.c number.c infinite_mul.c common_strings.c infinite_div.c infinite_mod.c common_multiply.c common_divide.c)
