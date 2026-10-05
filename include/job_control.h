#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

#include <sys/types.h>

void init_job_control(void);

void foreground_job(int job_id);
void background_job(int job_id);

pid_t get_shell_pgid(void);

#endif
