# Boolean Algebraic Functions and Digital Logic Circuits Tool

## Table of Contents

- [Organization of the Repository](#organization-of-the-repository)
- [Description of the Tool](#description-of-the-tool)
- [How to Read the Code](#how-to-read-the-code)
- [Compile Instructions](#compile-instructions)

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

## Description of the Tool

This tool is designed for manipulating Boolean algebraic functions and digital combinational logic circuits. It provides functionalities for converting Boolean expressions into canonical forms, minimizing them using prime implicants, and more.

## How to Read the Code

1. **Start with the Headers**: The header files in the `include` folder provide an overview of the classes and functions in the project. 
2. **Main Logic**: The `program.cpp` file in the `src` directory contains the main logic of the application, including handling user input and choices for operations.
3. **Boolean Operations**: For understanding how Boolean expressions are manipulated, refer to `specs.cpp`.
4. **Utilities and Input**: The `tools.cpp` and `input.cpp` files contain implementations for utility functions and input parsing, respectively.

## Compile Instructions
Use `-std=c++20` compile flag for best results.
