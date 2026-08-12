#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/lexer.h"

#define INITIAL_CAPACITY 16

static void add_token(TokenList *list, Token *token)
{
    if (list->count >= list->capacity)
    {
        list->capacity *= 2;

        list->tokens = realloc(
            list->tokens,
            list->capacity * sizeof(Token *)
        );

        if (list->tokens == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }

    list->tokens[list->count++] = token;
}

static char *make_string(const char *start, int length)
{
    char *str = malloc(length + 1);

    if (str == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(str, start, length);
    str[length] = '\0';

    return str;
}

TokenList *lexer_tokenize(const char *input)
{
    TokenList *list = malloc(sizeof(TokenList));

    if (list == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    list->count = 0;
    list->capacity = INITIAL_CAPACITY;

    list->tokens = malloc(
        list->capacity * sizeof(Token *)
    );

    if (list->tokens == NULL)
    {
        perror("malloc");
        free(list);
        exit(EXIT_FAILURE);
    }

    int i = 0;

    while (input[i] != '\0')
    {
        /* Skip whitespace */
        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|')
        {
            add_token(
                list,
                create_token(TOKEN_PIPE, "|")
            );

            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<')
        {
            add_token(
                list,
                create_token(TOKEN_REDIRECT_IN, "<")
            );

            i++;
            continue;
        }

        /* Output redirection */
        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                add_token(
                    list,
                    create_token(TOKEN_REDIRECT_APPEND, ">>")
                );

                i += 2;
            }
            else
            {
                add_token(
                    list,
                    create_token(TOKEN_REDIRECT_OUT, ">")
                );

                i++;
            }

            continue;
        }

        /* Background */
        if (input[i] == '&')
        {
            add_token(
                list,
                create_token(TOKEN_BACKGROUND, "&")
            );

            i++;
            continue;
        }

        /* Semicolon */
        if (input[i] == ';')
        {
            add_token(
                list,
                create_token(TOKEN_SEMICOLON, ";")
            );

            i++;
            continue;
        }

        /* Error redirection 2> */
        if (input[i] == '2' && input[i + 1] == '>')
        {
            add_token(
                list,
                create_token(TOKEN_REDIRECT_ERROR, "2>")
            );

            i += 2;
            continue;
        }

        /*
         * Word
         *
         * Everything until whitespace or a shell
         * operator is considered a word.
         */
        int start = i;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>' &&
               input[i] != '&' &&
               input[i] != ';')
        {
            i++;
        }

        int length = i - start;

        if (length > 0)
        {
            char *word = make_string(
                input + start,
                length
            );

            add_token(
                list,
                create_token(TOKEN_WORD, word)
            );

            free(word);
        }
    }

    add_token(
        list,
        create_token(TOKEN_EOF, NULL)
    );

    return list;
}

void free_token_list(TokenList *list)
{
    if (list == NULL)
        return;

    for (int i = 0; i < list->count; i++)
    {
        free_token(list->tokens[i]);
    }

    free(list->tokens);
    free(list);
}

void print_tokens(TokenList *list)
{
    if (list == NULL)
        return;

    printf("\n========== TOKENS ==========\n");

    for (int i = 0; i < list->count; i++)
    {
        Token *token = list->tokens[i];

        if (token->type == TOKEN_EOF)
        {
            printf("[%d] %-18s\n",
                   i,
                   token_type_to_string(token->type));
        }
        else
        {
            printf("[%d] %-18s : %s\n",
                   i,
                   token_type_to_string(token->type),
                   token->value);
        }
    }

    printf("============================\n\n");
}
