#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/executor.h"


int execute_external(Command *command)
{
    if (command == NULL || command->argc == 0)
        return 0;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        execvp(command->argv[0], command->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    if (command->background)
    {
        printf("[Background PID: %d]\n", pid);
        return 0;
    }

    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return 1;
}

int execute_pipeline(Command **commands, int command_count)
{
    if (commands == NULL || command_count <= 0)
        return 1;

    if (command_count != 2)
    {
        fprintf(stderr, "Only 2-command pipelines are supported currently.\n");
        return 1;
    }

    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    /* First command: stdout -> pipe */

    pid_t pid1 = fork();

    if (pid1 < 0)
    {
        perror("fork");

        close(pipefd[0]);
        close(pipefd[1]);

        return 1;
    }

    if (pid1 == 0)
    {
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        execvp(commands[0]->argv[0], commands[0]->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /* Second command: pipe -> stdin */

    pid_t pid2 = fork();

    if (pid2 < 0)
    {
        perror("fork");

        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid1, NULL, 0);

        return 1;
    }

    if (pid2 == 0)
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        /*
         * stdout is NOT changed.
         * Therefore the second command prints to the terminal.
         */

        close(pipefd[0]);
        close(pipefd[1]);

        execvp(commands[1]->argv[0], commands[1]->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /* Parent does not use the pipe */

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}

