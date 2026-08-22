#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expand.h"
#include "../include/builtin.h"
#include "../include/executor.h"

int main(void)
{
    while (1)
    {
        char *input = readline("shellforge$ ");

        if (input == NULL)
        {
            printf("\n");
            break;
        }

        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        add_history(input);

        TokenList *tokens = lexer_tokenize(input);

        print_tokens(tokens);

        CommandList *commands = parse_tokens(tokens);

        expand_command_list(commands);

        print_commands(commands);

        for (int i = 0; i < commands->count; i++)
        {
            Command *command = commands->commands[i];

            if (command->argc == 0)
                continue;

            if (is_builtin(command))
            {
                execute_builtin(command);
            }
            else
            {
                execute_external(command);
            }
        }

        free_command_list(commands);
        free_token_list(tokens);
        free(input);
    }

    return 0;
}
