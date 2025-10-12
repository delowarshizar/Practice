#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

// List of keywords
char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", "include", "define", "main", "printf", "scanf"
};
int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Check if a string is a keyword
int isKeyword(char str[])
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Main Lexical Analyzer
void lexicalAnalyzer(FILE *fp)
{
    char c;
    char token[MAX];
    int k = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        // Ignore whitespace
        if (isspace(c)) continue;

        // Handle comments
        if (c == '/')
        {
            char next = fgetc(fp);
            // Single-line comment
            if (next == '/')
            {
                k = 0;
                token[k++] = '/';
                token[k++] = '/';
                while ((c = fgetc(fp)) != '\n' && c != EOF && k < MAX - 1)
                {
                    token[k++] = c;
                }
                token[k] = '\0';
                printf("Token: %-30s \tType: SINGLE_LINE_COMMENT\n", token);
                continue;
            }
            // Multi-line comment
            else if (next == '*')
            {
                k = 0;
                token[k++] = '/';
                token[k++] = '*';
                int endFound = 0;

                while ((c = fgetc(fp)) != EOF && k < MAX - 2)
                {
                    token[k++] = c;
                    if (c == '*' && (next = fgetc(fp)) == '/')
                    {
                        token[k++] = '/';
                        endFound = 1;
                        break;
                    }
                    else if (c == '*' && next != '/')
                    {
                        ungetc(next, fp);  // only put back if not end of comment
                    }
                }

                token[k] = '\0';

                if (endFound)
                    printf("Token: %-30s \tType: MULTI_LINE_COMMENT\n", token);
                else
                    printf("Token: %-30s \tType: UNCLOSED_COMMENT\n", token);

                continue;
        }
        else
        {
            // It�s just a division or operator
            ungetc(next, fp);
        }
    }

    // Identifiers or keywords
    if (isalpha(c) || c == '_')
    {
        k = 0;
        token[k++] = c;
        while ((isalnum(c = fgetc(fp)) || c == '_') && k < MAX - 1)
        {
            token[k++] = c;
        }
        token[k] = '\0';
        if (c != EOF) ungetc(c, fp);

        if (isKeyword(token))
            printf("Token: %-30s \tType: KEYWORD\n", token);
        else
            printf("Token: %-30s \tType: IDENTIFIER\n", token);
    }

    // Numbers (integer or float)
    else if (isdigit(c))
    {
        k = 0;
        int hasDecimal = 0;
        token[k++] = c;
        while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.'))
        {
            if (c == '.') hasDecimal = 1;
            token[k++] = c;
        }
        token[k] = '\0';
        if (c != EOF) ungetc(c, fp);

        if (hasDecimal)
            printf("Token: %-30s \tType: FLOAT\n", token);
        else
            printf("Token: %-30s \tType: INTEGER\n", token);
    }

    // String literals
    else if (c == '"')
    {
        k = 0;
        token[k++] = '"';
        while ((c = fgetc(fp)) != '"' && c != EOF && k < MAX - 1)
        {
            token[k++] = c;
        }
        token[k++] = '"';
        token[k] = '\0';
        printf("Token: %-30s \tType: STRING_LITERAL\n", token);
    }

    // Character literals
    else if (c == '\'')
    {
        k = 0;
        token[k++] = '\'';
        while ((c = fgetc(fp)) != '\'' && c != EOF && k < MAX - 1)
        {
            token[k++] = c;
        }
        token[k++] = '\'';
        token[k] = '\0';
        printf("Token: %-30s \tType: CHAR_LITERAL\n", token);
    }

    // Multi-character operators
    else if (strchr("=<>!+-*&|/%", c))
    {
        char next = fgetc(fp);
        token[0] = c;
        token[1] = '\0';

        if ((c == '=' && next == '=') ||
                (c == '!' && next == '=') ||
                (c == '+' && next == '+') ||
                (c == '-' && next == '-') ||
                (c == '&' && next == '&') ||
                (c == '|' && next == '|') ||
                (c == '+' && next == '=') ||
                (c == '-' && next == '=') ||
                (c == '<' && next == '=') ||
                (c == '>' && next == '='))
        {
            token[1] = next;
            token[2] = '\0';
        }
        else
        {
            if (next != EOF) ungetc(next, fp);
        }

        printf("Token: %-30s \tType: OPERATOR\n", token);
    }

    // Special symbols
    else if (ispunct(c))
    {
        token[0] = c;
        token[1] = '\0';
        printf("Token: %-30s \tType: SPECIAL_SYMBOL\n", token);
    }
}
}

int main()
{
    FILE *fp = fopen("hello.c", "r");
    if (!fp)
    {
        printf("Cannot open file\n");
        return 1;
    }

    printf("Lexical Analysis Output:\n\n");
    lexicalAnalyzer(fp);

    fclose(fp);
    return 0;
}

