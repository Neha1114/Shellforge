#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    printf("==========================\n");
    printf(" Welcome to Shellforge\n");
    printf("==========================\n");

    char *line;

    while (1)
    {
        line = readline("shellforge> ");

        if (line == NULL)
        {
            printf("\nClosing Shellforge...\n");
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        add_history(line);

        if (strcmp(line, "exit") == 0)
        {
            printf("Closing Shellforge...\n");
            free(line);
            break;
        }

        printf("Command entered : %s\n", line);

        free(line);
    }

    return 0;
}
