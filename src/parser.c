#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"

#define INITIAL_COMMAND_CAPACITY 8
#define INITIAL_ARG_CAPACITY 8

static void add_argument(Command *command, const char *value)
{
    if (command->argc + 1 >= command->capacity)
    {
        command->capacity *= 2;

        command->argv = realloc(
            command->argv,
            command->capacity * sizeof(char *)
        );

        if (command->argv == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }

    command->argv[command->argc] = strdup(value);

    if (command->argv[command->argc] == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    command->argc++;

    command->argv[command->argc] = NULL;
}

Command *create_command(void)
{
    Command *command = malloc(sizeof(Command));

    if (command == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    command->argc = 0;
    command->capacity = INITIAL_ARG_CAPACITY;

    command->argv = malloc(
        command->capacity * sizeof(char *)
    );

    if (command->argv == NULL)
    {
        perror("malloc");
        free(command);
        exit(EXIT_FAILURE);
    }

    command->argv[0] = NULL;

    command->input_file = NULL;
    command->output_file = NULL;
    command->error_file = NULL;

    command->append_output = 0;
    command->background = 0;

    return command;
}

void free_command(Command *command)
{
    if (command == NULL)
        return;

    for (int i = 0; i < command->argc; i++)
    {
        free(command->argv[i]);
    }

    free(command->argv);

    free(command->input_file);
    free(command->output_file);
    free(command->error_file);

    free(command);
}

CommandList *create_command_list(void)
{
    CommandList *list = malloc(sizeof(CommandList));

    if (list == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    list->count = 0;
    list->capacity = INITIAL_COMMAND_CAPACITY;

    list->commands = malloc(
        list->capacity * sizeof(Command *)
    );

    if (list->commands == NULL)
    {
        perror("malloc");
        free(list);
        exit(EXIT_FAILURE);
    }

    return list;
}

void free_command_list(CommandList *list)
{
    if (list == NULL)
        return;

    for (int i = 0; i < list->count; i++)
    {
        free_command(list->commands[i]);
    }

    free(list->commands);
    free(list);
}

static void add_command(
    CommandList *list,
    Command *command
)
{
    if (list->count >= list->capacity)
    {
        list->capacity *= 2;

        list->commands = realloc(
            list->commands,
            list->capacity * sizeof(Command *)
        );

        if (list->commands == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }

    list->commands[list->count++] = command;
}

static char *copy_value(const char *value)
{
    if (value == NULL)
        return NULL;

    char *copy = strdup(value);

    if (copy == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    return copy;
}

CommandList *parse_tokens(TokenList *tokens)
{
    if (tokens == NULL)
        return NULL;

    CommandList *list = create_command_list();

    Command *current = create_command();

    for (int i = 0; i < tokens->count; i++)
    {
        Token *token = tokens->tokens[i];

        if (token->type == TOKEN_EOF)
        {
            break;
        }

        switch (token->type)
        {
            case TOKEN_WORD:
                add_argument(current, token->value);
                break;

            case TOKEN_REDIRECT_IN:
                if (i + 1 < tokens->count &&
                    tokens->tokens[i + 1]->type == TOKEN_WORD)
                {
                    i++;

                    free(current->input_file);

                    current->input_file =
                        copy_value(tokens->tokens[i]->value);
                }
                break;

            case TOKEN_REDIRECT_OUT:
                if (i + 1 < tokens->count &&
                    tokens->tokens[i + 1]->type == TOKEN_WORD)
                {
                    i++;

                    free(current->output_file);

                    current->output_file =
                        copy_value(tokens->tokens[i]->value);

                    current->append_output = 0;
                }
                break;

            case TOKEN_REDIRECT_APPEND:
                if (i + 1 < tokens->count &&
                    tokens->tokens[i + 1]->type == TOKEN_WORD)
                {
                    i++;

                    free(current->output_file);

                    current->output_file =
                        copy_value(tokens->tokens[i]->value);

                    current->append_output = 1;
                }
                break;

            case TOKEN_REDIRECT_ERROR:
                if (i + 1 < tokens->count &&
                    tokens->tokens[i + 1]->type == TOKEN_WORD)
                {
                    i++;

                    free(current->error_file);

                    current->error_file =
                        copy_value(tokens->tokens[i]->value);
                }
                break;

            case TOKEN_PIPE:
                if (current->argc > 0)
                {
                    add_command(list, current);
                    current = create_command();
                }
                break;

            case TOKEN_BACKGROUND:
                current->background = 1;
                break;

            case TOKEN_SEMICOLON:
                if (current->argc > 0)
                {
                    add_command(list, current);
                    current = create_command();
                }
                break;

            default:
                break;
        }
    }

    if (current->argc > 0 ||
        current->input_file != NULL ||
        current->output_file != NULL ||
        current->error_file != NULL)
    {
        add_command(list, current);
    }
    else
    {
        free_command(current);
    }

    return list;
}

void print_commands(CommandList *list)
{
    if (list == NULL)
        return;

    printf("\n========== PIPELINE ==========\n\n");

    for (int i = 0; i < list->count; i++)
    {
        Command *command = list->commands[i];

        printf("Command %d\n", i + 1);
        printf("------------------------------\n");

        printf("Arguments\n");

        for (int j = 0; j < command->argc; j++)
        {
            printf("argv[%d] = %s\n",
                   j,
                   command->argv[j]);
        }

        if (command->input_file != NULL)
        {
            printf("Input      : %s\n",
                   command->input_file);
        }
        else
        {
            printf("Input      : None\n");
        }

        if (command->output_file != NULL)
        {
            printf("Output     : %s\n",
                   command->output_file);
        }
        else
        {
            printf("Output     : None\n");
        }

        printf("Append     : %s\n",
               command->append_output ? "Yes" : "No");

        printf("Background : %s\n",
               command->background ? "Yes" : "No");

        printf("================================\n");
    }
}
