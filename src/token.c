#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/token.h"

Token *create_token(TokenType type, const char *value)
{
    Token *token = malloc(sizeof(Token));

    if (token == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token->type = type;

    if (value != NULL)
    {
        token->value = strdup(value);

        if (token->value == NULL)
        {
            perror("strdup");
            free(token);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        token->value = NULL;
    }

    return token;
}

void free_token(Token *token)
{
    if (token == NULL)
        return;

    free(token->value);
    free(token);
}

const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_REDIRECT_IN:
            return "REDIRECT_IN";

        case TOKEN_REDIRECT_OUT:
            return "REDIRECT_OUT";

        case TOKEN_REDIRECT_APPEND:
            return "REDIRECT_APPEND";

        case TOKEN_REDIRECT_ERROR:
            return "REDIRECT_ERROR";

        case TOKEN_BACKGROUND:
            return "BACKGROUND";

        case TOKEN_SEMICOLON:
            return "SEMICOLON";

        case TOKEN_EOF:
            return "EOF";

        default:
            return "UNKNOWN";
    }
}
