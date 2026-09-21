#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
if(pid > 0)
    {
        printf("Parent PID: %d\n", getpid());
        printf("Child PID : %d\n", pid);
        close(fd[0]);
        char msg[] = "Hello Child";
        write(fd[1], msg, strlen(msg)+1);
        close(fd[1]);
        wait(NULL);
    }
    else
    {
        close(fd[1]);
        char buffer[100];
        sleep(30);
        read(fd[0], buffer, sizeof(buffer));
        printf("Child Received: %s\n", buffer);
        close(fd[0]);
    }
    return 0;
}
