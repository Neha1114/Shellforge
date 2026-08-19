#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        // Child process
        printf("Child Process\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());

        sleep(10);

        printf("\nAfter parent exits:\n");
        printf("Child PID  : %d\n", getpid());
        printf("New PPID   : %d\n", getppid());

        return 0;
    }
    else
    {
        // Parent process
        printf("Parent Process\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n", pid);

        sleep(2);

        printf("Parent exiting...\n");
        return 0;
    }
}
