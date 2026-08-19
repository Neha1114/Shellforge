#include <stdio.h>

int main()
{
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[n];
    int bt[n];
    int remaining[n];
    int ct[n];
    int tat[n];
    int wt[n];

    printf("\nEnter Arrival Time and Burst Time for each process:\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);

        remaining[i] = bt[i];
    }

    int quantum;

    printf("\nEnter time quantum: ");
    scanf("%d", &quantum);

    int time = 0;
    int completed = 0;

    printf("\nRound Robin Gantt Chart:\n");

    printf("0");

    while (completed < n)
    {
        int executed = 0;

        for (int i = 0; i < n; i++)
        {
            if (remaining[i] > 0 && at[i] <= time)
            {
                executed = 1;

                printf(" | P%d |", i + 1);

                if (remaining[i] > quantum)
                {
                    time += quantum;
                    remaining[i] -= quantum;
                }
                else
                {
                    time += remaining[i];

                    remaining[i] = 0;

                    ct[i] = time;

                    completed++;
                }

                printf(" %d", time);
            }
        }

        /*
           If no process has arrived yet,
           move time forward
        */

        if (executed == 0)
        {
            time++;
        }
    }

    printf("\n");

    float total_wt = 0;
    float total_tat = 0;

    for (int i = 0; i < n; i++)
    {
        tat[i] = ct[i] - at[i];

        wt[i] = tat[i] - bt[i];

        total_wt += wt[i];
        total_tat += tat[i];
    }

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i + 1,
               at[i],
               bt[i],
               ct[i],
               tat[i],
               wt[i]);
    }

    printf("\nAverage Waiting Time = %.2f",
           total_wt / n);

    printf("\nAverage Turnaround Time = %.2f\n",
           total_tat / n);

    return 0;
}
