#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// List of C keywords
static const char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

// Check whether a word is a C keyword
int is_keyword(const char *word)
{
    size_t i;

    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// Check whether a character is an operator
int is_operator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
        ch == '%' || ch == '=' || ch == '<' || ch == '>' ||
        ch == '!' || ch == '&' || ch == '|' || ch == '^' ||
        ch == '~')
    {
        return 1;
    }

    return 0;
}

// Check whether a character is a special character
int is_special_character(char ch)
{
    if (ch == '(' || ch == ')' ||
        ch == '{' || ch == '}' ||
        ch == '[' || ch == ']' ||
        ch == ';' || ch == ',' ||
        ch == ':' || ch == '.' ||
        ch == '?')
    {
        return 1;
    }

    return 0;
}

// Convert token type to string
const char *token_type_string(TokenType type)
{
    switch (type)
    {
        case KEYWORD:
            return "KEYWORD";

        case IDENTIFIER:
            return "IDENTIFIER";

        case CONSTANT:
            return "CONSTANT";

        case OPERATOR:
            return "OPERATOR";

        case SPECIAL_CHARACTER:
            return "SPECIAL CHARACTER";

        case STRING_LITERAL:
            return "STRING";

        case CHARACTER_CONSTANT:
            return "CHARACTER CONSTANT";

        case PREPROCESSOR:
            return "PREPROCESSOR";

        case SINGLE_LINE_COMMENT:
            return "SINGLE-LINE-COMMENT";

        case MULTI_LINE_COMMENT:
            return "MULTI-LINE-COMMENT";

        default:
            return "INVALID_TOKEN";
    }
}

// Print one token
static void print_token(int line, const char *lexeme, TokenType type)
{
    printf("%-8d %-25s :  %s\n",
           line,
           token_type_string(type),
           lexeme);
}

// Main lexical analyzer
void lexical_analyzer(FILE *fp)
{
    int ch;
    int next;

    int line = 1;
    size_t index;

    char word[200];
    char operator[4];

    while ((ch = fgetc(fp)) != EOF)
    {
        // New line
        if (ch == '\n')
        {
            line++;
            continue;
        }

        // Ignore spaces, tabs and carriage return
        if (ch == ' ' || ch == '\t' || ch == '\r')
        {
            continue;
        }

        // Preprocessor directive
        if (ch == '#')
        {
            int start_line = line;

            index = 0;
            word[index++] = ch;

            while ((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }
            }

            word[index] = '\0';

            print_token(start_line, word, PREPROCESSOR);

            if (ch == '\n')
            {
                line++;
            }

            continue;
        }

        // Comments / division operator
        if (ch == '/')
        {
            next = fgetc(fp);

            // Single-line comment
            if (next == '/')
            {
                int start_line = line;

                index = 0;
                word[index++] = '/';
                word[index++] = '/';

                while ((ch = fgetc(fp)) != EOF && ch != '\n')
                {
                    if (index < sizeof(word) - 1)
                    {
                        word[index++] = ch;
                    }
                }

                word[index] = '\0';

                print_token(start_line,
                            word,
                            SINGLE_LINE_COMMENT);

                if (ch == '\n')
                {
                    line++;
                }

                continue;
            }

            // Multi-line comment
            if (next == '*')
            {
                int start_line = line;

                index = 0;
                word[index++] = '/';
                word[index++] = '*';

                while ((ch = fgetc(fp)) != EOF)
                {
                    if (index < sizeof(word) - 1)
                    {
                        word[index++] = ch;
                    }

                    if (ch == '\n')
                    {
                        line++;
                    }

                    if (ch == '*')
                    {
                        next = fgetc(fp);

                        if (next == '/')
                        {
                            if (index < sizeof(word) - 1)
                            {
                                word[index++] = '/';
                            }

                            break;
                        }

                        if (next != EOF)
                        {
                            ungetc(next, fp);
                        }
                    }
                }

                word[index] = '\0';

                print_token(start_line,
                            word,
                            MULTI_LINE_COMMENT);

                continue;
            }

            // Normal division operator
            operator[0] = '/';
            operator[1] = '\0';

            print_token(line, operator, OPERATOR);

            if (next != EOF)
            {
                ungetc(next, fp);
            }

            continue;
        }

        // String literal
        if (ch == '"')
        {
            index = 0;
            word[index++] = '"';

            while ((ch = fgetc(fp)) != EOF)
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }

                // Handle escape character
                if (ch == '\\')
                {
                    next = fgetc(fp);

                    if (next != EOF &&
                        index < sizeof(word) - 1)
                    {
                        word[index++] = next;
                    }

                    continue;
                }

                // Closing quote
                if (ch == '"')
                {
                    break;
                }

                if (ch == '\n')
                {
                    line++;
                }
            }

            word[index] = '\0';

            print_token(line, word, STRING_LITERAL);

            continue;
        }

        // Character constant
        if (ch == '\'')
        {
            index = 0;
            word[index++] = '\'';

            while ((ch = fgetc(fp)) != EOF)
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }

                // Handle escape character
                if (ch == '\\')
                {
                    next = fgetc(fp);

                    if (next != EOF &&
                        index < sizeof(word) - 1)
                    {
                        word[index++] = next;
                    }

                    continue;
                }

                // Closing quote
                if (ch == '\'')
                {
                    break;
                }
            }

            word[index] = '\0';

            print_token(line,
                        word,
                        CHARACTER_CONSTANT);

            continue;
        }

        // Identifier / keyword
        if (isalpha((unsigned char)ch) || ch == '_')
        {
            index = 0;

            while (isalnum((unsigned char)ch) || ch == '_')
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }

                ch = fgetc(fp);
            }

            word[index] = '\0';

            if (is_keyword(word))
            {
                print_token(line, word, KEYWORD);
            }
            else
            {
                print_token(line, word, IDENTIFIER);
            }

            if (ch != EOF)
            {
                ungetc(ch, fp);
            }

            continue;
        }

        // Constants
        if (isdigit((unsigned char)ch))
        {
            index = 0;

            // Hexadecimal constant
            if (ch == '0')
            {
                word[index++] = ch;

                next = fgetc(fp);

                if (next == 'x' || next == 'X')
                {
                    word[index++] = next;

                    ch = fgetc(fp);

                    while (isxdigit((unsigned char)ch))
                    {
                        if (index < sizeof(word) - 1)
                        {
                            word[index++] = ch;
                        }

                        ch = fgetc(fp);
                    }

                    word[index] = '\0';

                    print_token(line, word, CONSTANT);

                    if (ch != EOF)
                    {
                        ungetc(ch, fp);
                    }

                    continue;
                }

                if (next != EOF)
                {
                    ungetc(next, fp);
                }

                index = 0;
            }

            // Integer part
            while (isdigit((unsigned char)ch))
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }

                ch = fgetc(fp);
            }

            // Floating-point part
            if (ch == '.')
            {
                if (index < sizeof(word) - 1)
                {
                    word[index++] = ch;
                }

                ch = fgetc(fp);

                while (isdigit((unsigned char)ch))
                {
                    if (index < sizeof(word) - 1)
                    {
                        word[index++] = ch;
                    }

                    ch = fgetc(fp);
                }
            }

            word[index] = '\0';

            print_token(line, word, CONSTANT);

            if (ch != EOF)
            {
                ungetc(ch, fp);
            }

            continue;
        }

        // Operators
        if (is_operator(ch))
        {
            operator[0] = ch;
            operator[1] = '\0';
            operator[2] = '\0';

            next = fgetc(fp);

            // Two-character operators
            if ((ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && next == '=') ||
                (ch == '>' && next == '=') ||
                (ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|') ||
                (ch == '+' && next == '=') ||
                (ch == '-' && next == '=') ||
                (ch == '*' && next == '=') ||
                (ch == '/' && next == '=') ||
                (ch == '%' && next == '=') ||
                (ch == '&' && next == '=') ||
                (ch == '|' && next == '=') ||
                (ch == '^' && next == '=') ||
                (ch == '<' && next == '<') ||
                (ch == '>' && next == '>'))
            {
                operator[1] = next;
                operator[2] = '\0';

                print_token(line, operator, OPERATOR);

                continue;
            }

            // Return unused character
            if (next != EOF)
            {
                ungetc(next, fp);
            }

            print_token(line, operator, OPERATOR);

            continue;
        }

        // Special characters
        if (is_special_character(ch))
        {
            word[0] = ch;
            word[1] = '\0';

            print_token(line,
                        word,
                        SPECIAL_CHARACTER);

            continue;
        }

        // Unknown character
        word[0] = ch;
        word[1] = '\0';

        print_token(line, word, UNKNOWN);
    }
}