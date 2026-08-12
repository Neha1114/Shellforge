#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../include/lexer.h"

int main(void)
{
    printf("\n");
    printf("=====================================\n");
    printf("          SHELLFORGE MILESTONE 2     \n");
    printf("        TOKENIZER + LEXER             \n");
    printf("=====================================\n\n");

    while (1)
    {
        char *input = readline("shellforge> ");

        if (input == NULL)
        {
            printf("\nExiting Shellforge...\n");
            break;
        }

        /* Ignore empty input */
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        /* Add command to history */
        add_history(input);

        /* Exit command */
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }

        /* Tokenize input */
        TokenList *tokens = lexer_tokenize(input);

        /* Display tokens */
        print_tokens(tokens);

        /* Free memory */
        free_token_list(tokens);
        free(input);
    }

    printf("Shellforge terminated.\n");

    return 0;
}
