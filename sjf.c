#include <stdio.h>

int main()
{
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int bt[n];
    int process[n];
    int ct[n];
    int tat[n];
    int wt[n];

    printf("\nEnter burst time for each process:\n");

    for (int i = 0; i < n; i++)
    {
        process[i] = i + 1;

        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    /*
       Sort processes according to
       shortest burst time
    */

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (bt[i] > bt[j])
            {
                int temp;

                // Swap burst time
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                // Swap process number
                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    int time = 0;

    printf("\nSJF Execution Order:\n");

    printf("Gantt Chart:\n");

    printf("0");

    for (int i = 0; i < n; i++)
    {
        printf(" | P%d |", process[i]);

        time += bt[i];

        ct[i] = time;
        tat[i] = ct[i];
        wt[i] = tat[i] - bt[i];

        printf(" %d", time);
    }

    printf("\n");

    float total_wt = 0;
    float total_tat = 0;

    printf("\nProcess\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               process[i],
               bt[i],
               ct[i],
               tat[i],
               wt[i]);

        total_wt += wt[i];
        total_tat += tat[i];
    }

    printf("\nAverage Waiting Time = %.2f",
           total_wt / n);

    printf("\nAverage Turnaround Time = %.2f\n",
           total_tat / n);

    return 0;
}
