cmake_minimum_required(VERSION 3.12)
project(Bistromathique C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(Bistromathique src/main.c src/common.c src/eval_expr.c src/compute.c src/expression.c src/common_checks.c src/expression_stack.c src/operation_map.c src/infinite_add.c src/common_numbers.c src/infinite_sub.c src/number.c src/infinite_mul.c src/common_strings.c src/infinite_div.c src/infinite_mod.c src/common_multiply.c src/common_divide.c src/common_expression.c)
