#include <stdio.h>
#include <unistd.h>

int global_var = 100;

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // Child Process (Imagine VLC)
        global_var = 200;

        printf("\nChild Process (VLC)\n");
        printf("PID = %d\n", getpid());
        printf("global_var = %d\n", global_var);
        printf("Address of global_var = %p\n", &global_var);
    }
    else
    {
        // Parent Process (Imagine Chrome)
        sleep(1);

        printf("\nParent Process (Chrome)\n");
        printf("PID = %d\n", getpid());
        printf("global_var = %d\n", global_var);
        printf("Address of global_var = %p\n", &global_var);
    }

    return 0;
}
