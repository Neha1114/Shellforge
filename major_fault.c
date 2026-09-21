
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 4096

int main()
{
    int fd = open("page.txt", O_RDONLY);

    // Tell Linux to drop this file's cached pages
    posix_fadvise(fd, 0, SIZE, POSIX_FADV_DONTNEED);

    char *p = mmap(NULL, SIZE, PROT_READ,
                   MAP_PRIVATE, fd, 0);

    printf("Accessing page...\n");

    printf("Data = %c\n", p[0]);

    munmap(p, SIZE);
    close(fd);

    return 0;
}
