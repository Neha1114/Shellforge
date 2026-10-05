#include <stdio.h>
#include <string.h>

#include "../include/jobs.h"

#define MAX_JOBS 100

static Job job_table[MAX_JOBS];
static int job_count = 0;

static int next_job_id = 1;

void add_job(pid_t pid, const char *command)
{
    if (job_count >= MAX_JOBS)
    {
        fprintf(stderr, "Job table is full\n");
        return;
    }

    Job *job = &job_table[job_count];

    job->job_id = next_job_id++;
    job->pid = pid;
    job->pgid = pid;
    job->state = JOB_RUNNING;

    strncpy(job->command, command, sizeof(job->command) - 1);
    job->command[sizeof(job->command) - 1] = '\0';

    job_count++;

    printf("[%d] %d\n", job->job_id, job->pid);
}

void add_stopped_job(pid_t pid, pid_t pgid, const char *command)
{
    if (job_count >= MAX_JOBS)
    {
        fprintf(stderr, "Job table is full\n");
        return;
    }

    Job *job = &job_table[job_count];

    job->job_id = next_job_id++;
    job->pid = pid;
    job->pgid = pgid;
    job->state = JOB_STOPPED;

    strncpy(job->command, command, sizeof(job->command) - 1);
    job->command[sizeof(job->command) - 1] = '\0';

    job_count++;

    printf("[%d] Stopped    %s\n", job->job_id, job->command);
}

void remove_job(pid_t pid)
{
    for (int i = 0; i < job_count; i++)
    {
        if (job_table[i].pid == pid)
        {
            for (int j = i; j < job_count - 1; j++)
            {
                job_table[j] = job_table[j + 1];
            }

            job_count--;
            return;
        }
    }
}

void list_jobs(void)
{
    printf("ID\tSTATUS\t\tCOMMAND\n");

    for (int i = 0; i < job_count; i++)
    {
        const char *status;

        if (job_table[i].state == JOB_RUNNING)
            status = "Running";
        else if (job_table[i].state == JOB_STOPPED)
            status = "Stopped";
        else
            status = "Done";

        printf("[%d]\t%s\t\t%s\n",
               job_table[i].job_id,
               status,
               job_table[i].command);
    }
}

Job *find_job(int job_id)
{
    for (int i = 0; i < job_count; i++)
    {
        if (job_table[i].job_id == job_id)
            return &job_table[i];
    }

    return NULL;
}
