#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>

#include "../include/executor.h"
#include "../include/jobs.h"
#include "../include/job_control.h"

static void build_command_text(Command *command, char *buffer, size_t size)
{
    buffer[0] = '\0';

    for (int i = 0; i < command->argc; i++)
    {
        if (i > 0)
        {
            strncat(buffer, " ",
                    size - strlen(buffer) - 1);
        }

        strncat(buffer,
                command->argv[i],
                size - strlen(buffer) - 1);
    }
}

int execute_external(Command *command)
{
    if (command == NULL || command->argc == 0)
        return 0;

    char command_text[256];

    build_command_text(command,
                       command_text,
                       sizeof(command_text));

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    /*
     * CHILD
     */
    if (pid == 0)
    {
        /*
         * Put child into its own process group.
         */
        setpgid(0, 0);

        /*
         * Restore default signal handling.
         */
        signal(SIGTSTP, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        /*
         * If foreground command,
         * child will receive terminal signals
         * because parent gives terminal control
         * to this process group.
         */

        execvp(command->argv[0], command->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * PARENT
     */

    /*
     * Make child's process group.
     */
    setpgid(pid, pid);

    /*
     * BACKGROUND COMMAND
     *
     * Example:
     *
     * sleep 30 &
     */
    if (command->background)
    {
        add_job(pid, command_text);

        printf("[Background PID: %d]\n", pid);

        return 0;
    }

    /*
     * FOREGROUND COMMAND
     *
     * Example:
     *
     * sleep 30
     */

    /*
     * Give terminal control to child.
     */
    tcsetpgrp(STDIN_FILENO, pid);

    int status;

    /*
     * WUNTRACED is VERY IMPORTANT.
     *
     * It allows us to detect Ctrl+Z.
     */
    if (waitpid(pid, &status, WUNTRACED) == -1)
    {
        perror("waitpid");

        tcsetpgrp(STDIN_FILENO,
                  get_shell_pgid());

        return 1;
    }

    /*
     * Take terminal back.
     */
    tcsetpgrp(STDIN_FILENO,
              get_shell_pgid());

    /*
     * Ctrl+Z stopped the process.
     */
    if (WIFSTOPPED(status))
    {
        add_stopped_job(pid,
                        pid,
                        command_text);

        return 0;
    }

    /*
     * Normal completion.
     */
    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    return 1;
}

int execute_pipeline(Command **commands, int command_count)
{
    if (commands == NULL || command_count <= 0)
        return 1;

    if (command_count != 2)
    {
        fprintf(stderr,
                "Only 2-command pipelines are supported currently.\n");

        return 1;
    }

    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    /*
     * FIRST COMMAND
     */

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
        setpgid(0, 0);

        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        execvp(commands[0]->argv[0],
               commands[0]->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * SECOND COMMAND
     */

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
        /*
         * Join first command's process group.
         */
        setpgid(0, pid1);

        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        execvp(commands[1]->argv[0],
               commands[1]->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Parent puts both processes in same group.
     */
    setpgid(pid1, pid1);
    setpgid(pid2, pid1);

    close(pipefd[0]);
    close(pipefd[1]);

    /*
     * Give terminal to pipeline.
     */
    tcsetpgrp(STDIN_FILENO, pid1);

    int status1;
    int status2;

    waitpid(pid1, &status1, WUNTRACED);
    waitpid(pid2, &status2, WUNTRACED);

    /*
     * Give terminal back to shell.
     */
    tcsetpgrp(STDIN_FILENO,
              get_shell_pgid());

    return 0;
}
