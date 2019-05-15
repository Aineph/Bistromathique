cmake_minimum_required(VERSION 3.12)
project(Bistromathique C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(Bistromathique main.c helpers.c eval_expr.c compute.c expression.c parsing_helpers.c expression_list.c operation_list.c infinite_add.c compute_helpers.c infinite_sub.c number.c infinite_mul.c number_list.c string_helpers.c)