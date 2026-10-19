# Lexical Analyzer in C

## Description

Lexical Analyzer is a C program developed to understand the lexical
analysis phase of a compiler.

It reads a C source file character by character and identifies and
classifies meaningful tokens.

The analyzer identifies keywords, identifiers, constants, operators,
special characters, string literals, character constants, comments and
preprocessor directives.

## Features

- Read C source file
- Validate `.c` input file
- Check empty file
- Identify C keywords
- Identify identifiers
- Identify integer constants
- Identify floating-point constants
- Identify hexadecimal constants
- Identify operators
- Identify two-character operators
- Identify special characters
- Identify string literals
- Identify character constants
- Identify single-line comments
- Identify multi-line comments
- Identify preprocessor directives
- Identify invalid/unknown characters
- Display line number
- Display token type
- Display lexeme

## Project Structure

```text
Lexical_Analyzer/

├── Makefile
├── README.md
├── include/
│   └── lexer.h
├── input/
│   ├── simple.c
│   └── test.c
├── output/
└── src/
    ├── lexer.c
    └── main.c

## How to test

1.Run make to build the project.
2.Run ./lexer input/test.c to test the comprehensive C file.
3.Run ./lexer input/simple.c to test the simple C file.
4.Verify the output for correct token type, lexeme, and line number.
5.Run make clean to remove the executable.