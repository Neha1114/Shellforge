#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

typedef enum
{
    JOB_RUNNING,
    JOB_STOPPED,
    JOB_DONE
} JobState;

typedef struct
{
    int job_id;
    pid_t pid;
    pid_t pgid;
    JobState state;
    char command[256];
} Job;

void add_job(pid_t pid, const char *command);
void add_stopped_job(pid_t pid, pid_t pgid, const char *command);

void remove_job(pid_t pid);

void list_jobs(void);

Job *find_job(int job_id);

#endif
