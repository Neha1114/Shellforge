#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expand.h"
#include "../include/builtin.h"
#include "../include/executor.h"
#include "../include/job_control.h"

int main(void)
{
    /*
     * Initialize shell job control.
     */
    init_job_control();

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

        /*
         * =========================
         * LEXICAL ANALYSIS
         * =========================
         */

        TokenList *tokens = lexer_tokenize(input);

        print_tokens(tokens);

        /*
         * =========================
         * PARSING
         * =========================
         */

        CommandList *commands = parse_tokens(tokens);

        /*
         * =========================
         * EXPANSION
         * =========================
         */

        expand_command_list(commands);

        print_commands(commands);

        /*
         * =========================
         * EXECUTION
         * =========================
         */

        if (commands->count > 1)
        {
            Command *pipeline_commands[2];

            pipeline_commands[0] =
                commands->commands[0];

            pipeline_commands[1] =
                commands->commands[1];

            execute_pipeline(
                pipeline_commands,
                2);
        }
        else
        {
            for (int i = 0;
                 i < commands->count;
                 i++)
            {
                Command *command =
                    commands->commands[i];

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

        /*
         * =========================
         * CLEANUP
         * =========================
         */

        free_command_list(commands);
        free_token_list(tokens);
        free(input);
    }

    return 0;
}
