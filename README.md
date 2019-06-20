# Bistromathique

This is the first big assignment at Epitech. Its purpose is to calculate numbers with infinite digits.
This version of Bistromathique contains the following operations:

* Infinite addition
* Infinite subtraction
* Infinite multiplication
* Infinite division
* Infinite modulo

## Build System

The project uses CMake in order to compile. The minimum version required is 3.12.
You can download it there: https://cmake.org/download/.

If you can't get the latest version of CMake, you can still try to change the value in the CMakeLists.txt.
You need at least a C compiler to build the binary.

This section will guide you on how to build the target using cmake.
After cloning the repository, create an empty "build" repository inside this project, by using the command:
```
$> mkdir build
```

Once the repository is created run the following commands:

```
$> cd build/
# If you are using a linux system (including macOS)
$> cmake .. && make
# If you want to compile it on an other platform
$> cmake .. -G [platform_name]
# This command is an example of how to launch the program
$> echo "3+3" | ./Bistromathique "0123456789" "()+-*/%" 3
```

## User Manual

Bistromathique takes 3 mandatory arguments from the command line:

* The base of the numbers to compute
* The list of operators
* The size of the expression to parse

#### The Base

You need to provide the program the base on which you want to calculate your expression.
The syntax of this argument is a string containing the characters of the base.

**Example:**
```
# Computes numbers with a base of 10 (decimal)
$> ./Bistromathique "0123456789" "()+-*/%" 42
# Computes numbers with a base of 2 (binary)
$> ./Bistromathique "01" "()+-*/%" 42
# Computes numbers with a base of 16 (hexadecimal)
$> ./Bistromathique "0123456789ABCDEF" "()+-*/%" 42
```

You can change every digits in order to create your own base of computation.

#### The Operators

The list of operators has a fixed size.
Thus you will necessarily have to provide the 7 mandatory operators the program needs in order to run it.
The operators you need to provide are:
1. The left parenthesis operator
2. The right parenthesis operator
3. The addition operator
4. The subtraction operator
5. The multiplication operator
6. The division operator
7. The modulo operator

**Example:**

```
# This is the normal syntax
$> ./Bistromathique "0123456789" "()+-*/%" 42
# Redefinition of the operators with an other syntax
$> ./Bistromathique "0123456789" "[]+-x:R" 42
```

#### The Size

You also have to provide the size of the arithmetic expression.
If you use the program using the "echo", then you should give the exact number of characters you entered.

**Example:**

```
# Computing the whole expression
$> echo "(4+5)*2" | ./Bistromathique "0123456789" "()+-*/%" 7
# Computing only the expression inside the parenthesis
$> echo "(4+5)*2" | ./Bistromathique "0123456789" "()+-*/%" 5
```

## Specifications

### Parsing

The program parses the expression using a binary tree.
For every operator found in the expression, a node is created to store the expression.
Considering the expression "4+5+(3-7)*2", the obtained expression tree will look like the following:

```
       4
      /
     +
    / \
   /   5
  /
+        3
  \     /
   \   -
    \ / \
     *   7
      \
        2
```

### Computations

The algorithms used in for calculations are standards naive algorithms for the addition and the subtraction.
They both have a complexity of O(n).

In order to improve the computation time of multiplications,
the Karatsuba algorithm is used. Thus instead of having a complexity of 0(n^2) as with naive multiplication algorithms,
the complexity is about O(n^log2(3)) which is almost equivalent to O(n^1.585).

Finally, the algorithm used for both the division and the modulo is the "Knuth" long division algorithm as described
in the book "The art of computer programming, Vol. 2" in the section 4.3.1.
