#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expand.h"
#include "../include/builtin.h"

int main(void)
{
    while (1)
    {
        char *input = readline("shellforge$ ");

        if (input == NULL)
        {
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

        if (commands->count == 1)
        {
            Command *command = commands->commands[0];

            if (is_builtin(command))
            {
                execute_builtin(command);
            }
            else
            {
                fprintf(stderr,
                        "%s: command not found\n",
                        command->argv[0]);
            }
        }
        else if (commands->count > 1)
        {
            fprintf(stderr,
                    "Pipelines are not executed in Milestone 3.1\n");
        }

        free_command_list(commands);
        free_token_list(tokens);
        free(input);
    }

    return 0;
}
