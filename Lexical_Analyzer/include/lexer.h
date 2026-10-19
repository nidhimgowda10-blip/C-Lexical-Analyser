#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

// Token types
typedef enum
{
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    OPERATOR,
    SPECIAL_CHARACTER,
    STRING_LITERAL,
    CHARACTER_CONSTANT,
    PREPROCESSOR,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    UNKNOWN
} TokenType;

// Token structure
typedef struct
{
    TokenType type;
    char lexeme[200];
    int line;
} Token;

// Start lexical analysis
void lexical_analyzer(FILE *fp);

// Check keyword
int is_keyword(const char *word);

// Check operator
int is_operator(char ch);

// Check special character
int is_special_character(char ch);

// Convert token type to string
const char *token_type_string(TokenType type);

#endif