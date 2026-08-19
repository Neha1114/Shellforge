#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/expand.h"

int main(void)
{
    printf("\n");
    printf("=====================================\n");
    printf("          SHELLFORGE MILESTONE 2.2  \n");
    printf("       PARSER + EXPAND               \n");
    printf("=====================================\n\n");

    while (1)
    {
        char *input = readline("shellforge$ ");

        if (input == NULL)
        {
            printf("\nExiting Shellforge...\n");
            break;
        }

        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        add_history(input);

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

	TokenList *tokens = lexer_tokenize(input);

	print_tokens(tokens);

	CommandList *commands = parse_tokens(tokens);

	expand_command_list(commands);

	printf("\nAfter expansion:\n");

	print_commands(commands);

	free_command_list(commands);
	free_token_list(tokens);
        free(input);
    }

    printf("Shellforge terminated.\n");

    return 0;
}
