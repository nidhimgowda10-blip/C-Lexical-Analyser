#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    FILE *fp;

    // Check command-line argument
    if (argc != 2)
    {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    // Check C file extension
    int length = 0;

    while (argv[1][length] != '\0')
    {
        length++;
    }

    if (length < 3 ||
        argv[1][length - 2] != '.' ||
        argv[1][length - 1] != 'c')
    {
        printf("Error: Input file must be a .c file\n");
        return 1;
    }

    // Open source file
    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open file %s\n", argv[1]);
        return 1;
    }

    // Check whether file is empty
    if (fgetc(fp) == EOF)
    {
        printf("Error: Input file is empty\n");
        fclose(fp);
        return 1;
    }

    // Move back to beginning
    rewind(fp);

    // Print header
    printf("\n%-8s %-25s :  %s\n",
           "LINE", "TOKEN_TYPE", "LEXEME");

    printf("------------------------------------------------------------\n");

    // Start lexical analysis
    lexical_analyzer(fp);

    // Close file
    fclose(fp);

    return 0;
}