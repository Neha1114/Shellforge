#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/wait.h>

#include "../include/jobs.h"
#include "../include/job_control.h"

static pid_t shell_pgid;

void init_job_control(void)
{
    shell_pgid = getpid();

    /*
     * Put the shell in its own process group.
     */
    if (setpgid(shell_pgid, shell_pgid) < 0)
    {
        /*
         * It may already be in its own process group.
         * That is okay.
         */
    }

    /*
     * Ignore terminal job-control signals in the shell.
     */
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    /*
     * Make the shell the foreground process group.
     */
    tcsetpgrp(STDIN_FILENO, shell_pgid);
}

pid_t get_shell_pgid(void)
{
    return shell_pgid;
}

void foreground_job(int job_id)
{
    Job *job = find_job(job_id);

    if (job == NULL)
    {
        printf("fg: job not found\n");
        return;
    }

    printf("Bringing job [%d] to foreground\n", job_id);

    /*
     * Give terminal control to the job.
     */
    tcsetpgrp(STDIN_FILENO, job->pgid);

    /*
     * Continue the stopped job.
     */
    if (job->state == JOB_STOPPED)
    {
        kill(-job->pgid, SIGCONT);
    }

    /*
     * Wait until the job exits OR stops again.
     */
    int status;

    if (waitpid(job->pid, &status, WUNTRACED) < 0)
    {
        perror("waitpid");
    }

    /*
     * Take terminal control back.
     */
    tcsetpgrp(STDIN_FILENO, shell_pgid);

    if (WIFSTOPPED(status))
    {
        job->state = JOB_STOPPED;

        printf("\n[%d] Stopped    %s\n",
               job->job_id,
               job->command);

        return;
    }

    if (WIFEXITED(status) || WIFSIGNALED(status))
    {
        remove_job(job->pid);
    }
}

void background_job(int job_id)
{
    Job *job = find_job(job_id);

    if (job == NULL)
    {
        printf("bg: job not found\n");
        return;
    }

    /*
     * Continue the entire process group.
     */
    if (kill(-job->pgid, SIGCONT) < 0)
    {
        perror("bg");
        return;
    }

    job->state = JOB_RUNNING;

    printf("[%d] Running    %s\n",
           job->job_id,
           job->command);
}
