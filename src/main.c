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

        /* =========================
         * LEXICAL ANALYSIS
         * ========================= */
        TokenList *tokens = lexer_tokenize(input);

        print_tokens(tokens);

        /* =========================
         * PARSING
         * ========================= */
        CommandList *commands = parse_tokens(tokens);

        /* =========================
         * EXPANSION
         * ========================= */
        expand_command_list(commands);

        print_commands(commands);

        /* =========================
         * EXECUTION
         * ========================= */

        /*
         * If there is more than one command,
         * it means we have a pipeline.
         *
         * Example:
         *
         *     ls | pwd
         *
         * commands->count == 2
         */
        if (commands->count > 1)
        {
            /*
             * For now, pipeline execution supports
             * two commands.
             */
            Command *pipeline_commands[2];

            pipeline_commands[0] = commands->commands[0];
            pipeline_commands[1] = commands->commands[1];

            execute_pipeline(pipeline_commands, 2);
        }
        else
        {
            /*
             * Only one command.
             *
             * Example:
             *
             *     ls
             *     pwd
             *     cat file.txt
             */
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
        }

        /* =========================
         * CLEANUP
         * ========================= */

        free_command_list(commands);
        free_token_list(tokens);
        free(input);
    }

    return 0;
}

