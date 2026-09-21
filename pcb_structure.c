#include <stdio.h>
#include <stdlib.h>

int x1;                  // Uninitialized Data Segment
int x2;                  // Uninitialized Data Segment

int global_var1 = 100;   // Data Segment
int global_var2 = 100;   // Data Segment

int main()
{
    int local_var1 = 200;    // Stack Segment
    int local_var2 = 200;    // Stack Segment

    int *heap_var1 = (int *)malloc(sizeof(int));
    *heap_var1 = 300;        // Heap Segment

    int *heap_var2 = (int *)malloc(sizeof(int));
    *heap_var2 = 300;        // Heap Segment

    printf("Code Segment          : %p\n", (void *)main);

    printf("Data Segment          : %p\n", (void *)&global_var1);
    printf("Data Segment          : %p\n", (void *)&global_var2);

    printf("Uninitialized Segment : %p\n", (void *)&x1);
    printf("Uninitialized Segment : %p\n", (void *)&x2);

    printf("Heap Segment          : %p\n", (void *)heap_var1);
    printf("Heap Segment          : %p\n", (void *)heap_var2);

    printf("Stack Segment         : %p\n", (void *)&local_var1);
    printf("Stack Segment         : %p\n", (void *)&local_var2);

    free(heap_var1);
    free(heap_var2);

    return 0;
}
