#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int main()
{
    key_t key = 1234;
    int shmid = shmget(key, 1024, 0666);

    if(shmid == -1)
    {
        printf("Shared memory not found\n");
        return 1;
    }
    shmctl(shmid, IPC_RMID, NULL);
    printf("Shared memory removed successfully\n");
    return 0;
}
