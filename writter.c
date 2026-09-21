#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
int main()
{
    key_t key = 1234;
    /* Create shared memory segment */
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if(shmid == -1)
    {
        printf("Shared memory creation failed\n");
        return 1;
    }
 /* Attach shared memory */
    char *ptr = (char *)shmat(shmid, NULL, 0);
    if(ptr == (char *)-1)
    {
        printf("Attach failed\n");
        return 1;
    }
    /* Write data */
    strcpy(ptr, "Hello from Writer Process");
    printf("Data Written: %s\n", ptr);
    /* Detach shared memory */
    shmdt(ptr);
    return 0;
}
