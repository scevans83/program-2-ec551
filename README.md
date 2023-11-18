# Boolean Algebraic Functions and Digital Logic Circuits Tool

Sophia Evans and Aidan Nowakowski

## Table of Contents

- [Description of the Tool](#description-of-the-tool)
- [Organization of the Repository](#organization-of-the-repository)
- [How to Read the Code](#how-to-read-the-code)
- [Compile Instructions](#compile-instructions)

## Description of the Tool

This tool is designed for manipulating Boolean algebraic functions and digital combinational logic circuits. It provides functionalities for converting Boolean expressions into canonical forms, minimizing them using prime implicants, and more.

### User Input

User has the option of inputting either:

1. A Boolean expression in (variables)(minterms) format (e.g. `(a,b,c)(1,3,4)`)
2. A digital combinational logic circuit in BLIF format

Example:

    .model simple_model
    .inputs a b c
    .outputs f
    .names a b c f
    001 1
    011 1
    100 1

    .end

### Parsing Input and Storing Values

The inputted description is stored as a `BooleanExpression`, which contains vectors for `variables`, `minterms`, and `maxterms`.

For a Boolean expression in minterms format, the tool extracts variables and minterms from the input string and stores them in the BooleanExpression object. The vector of maxterms is calculated by determining what possible values (given the number of variables) are not present in the minterms vector.

For BLIF format, the tool generates a truth table based on the number of input variables specified in the .inputs line. It then reads the .names line and the subsequent lines that define the minterms and maxterms of the expression. For each row in the truth table, the function checks if the row matches the minterm or maxterm and adds it to the corresponding vector in the BooleanExpression object.

### Primary Functionality

The user has the option to choose from 12 different functions:

1. Return the design as a canonical SOP
2. Return the design as a canonical POS
3. Return the design INVERSE as a canonical SOP
4. Return the design INVERSE as a canonical POS
5. Return a minimized number of literals representation in SOP
6. Return a minimized number of literals representation in POS
7. Report the number of Prime Implicants
8. Report the number of Essential Prime Implicants
9. Report the number of ON-Set minterms
10. Report the number of ON-Set maxterms
11. Draw SOP circuit as ASCII
12. Draw POS circuit as ASCII

## Organization of the Repository

The repository is organized into two main directories: `include` and `src`.

### `include`

This directory contains all the header files.

- `input.h`: Handles parsing of Boolean expressions and BLIF text.
- `program.h`: Contains the main program logic.
- `specs.h`: Contains function prototypes for canonical and minimized forms of Boolean expressions.
- `tools.h`: Contains utility functions for Boolean expressions and terms.

### `src`

This directory contains the implementation of the functions declared in the header files.

- `input.cpp`: Implements the functions for parsing Boolean expressions and BLIF text.
- `program.cpp`: Implements the main logic of the application.
- `specs.cpp`: Implements various forms of Boolean expression manipulation, such as constructing canonical and minimized forms.
- `tools.cpp`: Implements utility functions for manipulating the Boolean expressions.

## How to Read the Code

1. **Main Logic**: The `program.cpp` file in the `src` directory contains the main logic of the application, including handling user input and choices for operations.
2. **Boolean Operations**: For understanding how Boolean expressions are manipulated, refer to `specs.cpp`.
3. **Utilities and Input**: The `tools.cpp` and `input.cpp` files contain implementations for utility functions and input parsing, respectively.

## Compile Instructions

Use `-std=c++20` compile flag for best results.

(to run test.cpp: g++ -std=c++20 ./src/test.cpp ./src/LUT.cpp ./src/tools.cpp -o test)
(to run main.cpp: g++ -std=c++20 ./src/main.cpp -o main)

## References

For more information on BLIF:

- https://course.ece.cmu.edu/~ee760/760docs/blif.pdf
- http://bear.ces.cwru.edu/eecs_cad/sis_blif.pdf (slightly more recent version of above)
- https://rw1nkler-vtr-docs.readthedocs.io/en/latest/vpr/file_formats/

ASCII Circuits inspiration:

- https://codegolf.stackexchange.com/questions/22850/ascii-art-logic-gate-diagram
