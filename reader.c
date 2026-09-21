#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    key_t key = 1234;

    /* Access existing shared memory */
    int shmid = shmget(key, 1024, 0666);

    if(shmid == -1)
    {
        printf("Shared memory not found\n");
        return 1;
    }

 /* Attach shared memory */
    char *ptr = (char *)shmat(shmid, NULL, 0);

    if(ptr == (char *)-1)
    {
        printf("Attach failed\n");
        return 1;
    }

    /* Read data */
    printf("Data Read: %s\n", ptr);

    /* Detach shared memory */
    shmdt(ptr);

    return 0;
}
