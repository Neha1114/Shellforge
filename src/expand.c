#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/expand.h"

static void append_text(
    char **result,
    size_t *length,
    size_t *capacity,
    const char *text
)
{
    size_t text_length = strlen(text);

    while (*length + text_length + 1 > *capacity)
    {
        *capacity *= 2;

        *result = realloc(
            *result,
            *capacity
        );

        if (*result == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
    }

    memcpy(
        *result + *length,
        text,
        text_length
    );

    *length += text_length;

    (*result)[*length] = '\0';
}

char *expand_variable(const char *value)
{
    if (value == NULL)
        return NULL;

    size_t capacity = 64;
    size_t length = 0;

    char *result = malloc(capacity);

    if (result == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';

    size_t i = 0;

    while (value[i] != '\0')
    {
        if (value[i] == '$')
        {
            i++;

            if (value[i] == '?')
            {
                const char *status = getenv("?");

                if (status == NULL)
                    status = "0";

                append_text(
                    &result,
                    &length,
                    &capacity,
                    status
                );

                i++;
                continue;
            }

            if ((value[i] >= 'A' && value[i] <= 'Z') ||
                (value[i] >= 'a' && value[i] <= 'z') ||
                value[i] == '_')
            {
                char variable[256];
                int j = 0;

                while (
                    value[i] != '\0' &&
                    ((value[i] >= 'A' && value[i] <= 'Z') ||
                     (value[i] >= 'a' && value[i] <= 'z') ||
                     (value[i] >= '0' && value[i] <= '9') ||
                     value[i] == '_')
                )
                {
                    if (j < 255)
                    {
                        variable[j++] = value[i];
                    }

                    i++;
                }

                variable[j] = '\0';

                const char *env_value =
                    getenv(variable);

                if (env_value != NULL)
                {
                    append_text(
                        &result,
                        &length,
                        &capacity,
                        env_value
                    );
                }

                continue;
            }

            append_text(
                &result,
                &length,
                &capacity,
                "$"
            );

            continue;
        }

        char temp[2];

        temp[0] = value[i];
        temp[1] = '\0';

        append_text(
            &result,
            &length,
            &capacity,
            temp
        );

        i++;
    }

    return result;
}

void expand_command(Command *command)
{
    if (command == NULL)
        return;

    for (int i = 0; i < command->argc; i++)
    {
        char *expanded =
            expand_variable(command->argv[i]);

        free(command->argv[i]);

        command->argv[i] = expanded;
    }

    if (command->input_file != NULL)
    {
        char *expanded =
            expand_variable(command->input_file);

        free(command->input_file);

        command->input_file = expanded;
    }

    if (command->output_file != NULL)
    {
        char *expanded =
            expand_variable(command->output_file);

        free(command->output_file);

        command->output_file = expanded;
    }

    if (command->error_file != NULL)
    {
        char *expanded =
            expand_variable(command->error_file);

        free(command->error_file);

        command->error_file = expanded;
    }
}

void expand_command_list(CommandList *list)
{
    if (list == NULL)
        return;

    for (int i = 0; i < list->count; i++)
    {
        expand_command(list->commands[i]);
    }
}

