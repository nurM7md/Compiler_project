#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100

// Token types
typedef enum
{
    KEYWORD,
    SPECIAL_CHAR,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    END_OF_FILE
}
TokenType;

// Token structure
typedef struct
{
    TokenType type;
    char lexeme[MAX_TOKEN_LEN];
} Token;

// Function to check if a character is an operator
int isOperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '='  || ch == '>'  || ch == '<' || ch == '*';
}

// Function to check if a string is a keyword
int isKeyword(char *str)
{
    char *keywords[] = {"if", "else", "while", "for", "int", "float", "char", "return", "when"};

    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}


// Function to perform lexical analysis
// Function to perform lexical analysis
Token getNextToken(const char *input, int *pos)
{
    Token token;
    int i = *pos;

    // Skip whitespaces and newline characters
    while (isspace(input[i]) || input[i] == '\n')
    {
        i++;
    }

    if (input[i] == '\0')
    {
        // End of file
        token.type = END_OF_FILE;
        strcpy(token.lexeme, "EOF");
    }
    else if (isalpha(input[i]) || input[i] == '_')
    {
        // Identifier or keyword
        int j = 0;
        token.lexeme[j++] = input[i];

        while ((input[i] != '\0') && (isalnum(input[i]) || input[i] == '_'))
        {
            token.lexeme[j++] = input[i++];
        }

        token.lexeme[j] = '\0';  // Null-terminate the lexeme
        if (isKeyword(token.lexeme))
        {
            token.type = KEYWORD;
        }
        else
        {
            token.type = IDENTIFIER;
        }
    }
    else if (isdigit(input[i]))
    {
        // Number
        int j = 0;
        token.lexeme[j++] = input[i];

        while ((input[i] != '\0') && (isdigit(input[i]) || input[i] == '.'))
        {
            token.lexeme[j++] = input[i++];
        }

        token.lexeme[j] = '\0';  // Null-terminate the lexeme
        token.type = NUMBER;
    }
    else if (isOperator(input[i]))
    {
        // Operator
        token.type = OPERATOR;
        token.lexeme[0] = input[i];
        token.lexeme[1] = '\0';
        i++;
    }
    else
    {
        // Special character
        token.type = SPECIAL_CHAR;
        token.lexeme[0] = input[i++];
        token.lexeme[1] = '\0';
    }

    *pos = i;
    return token;
}


int main()
{
    printf("Enter a statement or arithmetic operation:\n");

    char input[MAX_TOKEN_LEN];
    fgets(input, sizeof(input), stdin);

    int pos = 0;
    Token token;

    do
    {
        token = getNextToken(input, &pos);

        switch (token.type)
        {
        case KEYWORD:
            printf("Keyword: %s\n", token.lexeme);
            break;
        case SPECIAL_CHAR:
            printf("Special Character: %s\n", token.lexeme);
            break;
        case IDENTIFIER:
            token.lexeme[strlen(token.lexeme) - 1] = '\0';  // Remove trailing newline
            printf("Identifier: %s\n", token.lexeme);
            break;
        case NUMBER:
            token.lexeme[strlen(token.lexeme) - 1] = '\0';  // Remove trailing newline
            printf("Number: %s\n", token.lexeme);
            break;

        case OPERATOR:
            printf("Operator: %s\n", token.lexeme);
            break;
        case END_OF_FILE:
            printf("End of File\n");
            break;
        default:
            break;
        }
    }
    while (input[pos] != '\0');

    return 0;
}

