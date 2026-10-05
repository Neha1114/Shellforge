#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "../include/builtin.h"
#include "../include/jobs.h"
#include "../include/job_control.h"

int is_builtin(Command *command)
{
    if (command == NULL || command->argc == 0)
        return 0;

    if (strcmp(command->argv[0], "cd") == 0)
        return 1;

    if (strcmp(command->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(command->argv[0], "echo") == 0)
        return 1;

    if (strcmp(command->argv[0], "exit") == 0)
        return 1;
    if (strcmp(command->argv[0], "jobs") == 0)
        return 1;
    if (strcmp(command->argv[0], "fg") == 0)
        return 1;

    if (strcmp(command->argv[0], "bg") == 0)
        return 1;
    return 0;
}

static int builtin_cd(Command *command)
{
    char *directory;

    if (command->argc == 1)
    {
        directory = getenv("HOME");

        if (directory == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else if (command->argc == 2)
    {
        directory = command->argv[1];
    }
    else
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    if (chdir(directory) != 0)
    {
        perror("cd");
        return 1;
    }

    return 0;
}

static int builtin_pwd(Command *command)
{
    (void)command;

    char current_directory[PATH_MAX];

    if (getcwd(current_directory, sizeof(current_directory)) == NULL)
    {
        perror("pwd");
        return 1;
    }

    printf("2500031711%s\n", current_directory);

    return 0;
}

static int builtin_echo(Command *command)
{
    for (int i = 1; i < command->argc; i++)
    {
        printf("%s", command->argv[i]);

        if (i < command->argc - 1)
            printf(" ");
    }

    printf("\n");

    return 0;
}

static int builtin_exit(Command *command)
{
    (void)command;

    exit(0);
}

int execute_builtin(Command *command)
{
    if (command == NULL || command->argc == 0)
        return 1;

    if (strcmp(command->argv[0], "cd") == 0)
        return builtin_cd(command);

    if (strcmp(command->argv[0], "pwd") == 0)
        return builtin_pwd(command);

    if (strcmp(command->argv[0], "echo") == 0)
        return builtin_echo(command);

    if (strcmp(command->argv[0], "exit") == 0)
        return builtin_exit(command);
    if (strcmp(command->argv[0], "jobs") == 0)
    {
        list_jobs();
        return 0;
    }
    if (strcmp(command->argv[0], "fg") == 0)
    {
        if (command->argc != 2)
        {
            printf("Usage: fg %%job_id\n");
            return 1;
        }

        int job_id = atoi(command->argv[1] + 1);

        foreground_job(job_id);

        return 0;
    }

    if (strcmp(command->argv[0], "bg") == 0)
    {
        if (command->argc != 2)
        {
            printf("Usage: bg %%job_id\n");
            return 1;
        }

        int job_id = atoi(command->argv[1] + 1);

        background_job(job_id);

        return 0;
    }

    return 1;
}
