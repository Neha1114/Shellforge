#include <stdio.h>

int main()
{
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[n];
    int bt[n];
    int process[n];
    int ct[n];
    int tat[n];
    int wt[n];

    printf("\nEnter Arrival Time and Burst Time for each process:\n");

    for (int i = 0; i < n; i++)
    {
        process[i] = i + 1;

        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);
    }

    /*
       Sort processes according to
       Arrival Time first
    */

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (at[i] > at[j])
            {
                int temp;

                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    int time = 0;
    int completed = 0;
    int visited[n];

    for (int i = 0; i < n; i++)
        visited[i] = 0;

    printf("\nSJF Execution Order:\n");
    printf("Gantt Chart:\n");

    while (completed < n)
    {
        int shortest = -1;

        /*
           Find the shortest burst time among
           processes that have arrived
        */

        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && at[i] <= time)
            {
                if (shortest == -1 || bt[i] < bt[shortest])
                {
                    shortest = i;
                }
            }
        }

        /*
           If no process has arrived yet,
           move the CPU time forward
        */

        if (shortest == -1)
        {
            time++;

            printf("%d ", time);
            continue;
        }

        printf("| P%d |", process[shortest]);

        time += bt[shortest];

        ct[shortest] = time;

        tat[shortest] = ct[shortest] - at[shortest];

        wt[shortest] = tat[shortest] - bt[shortest];

        visited[shortest] = 1;
        completed++;
    }

    printf("\n");

    float total_wt = 0;
    float total_tat = 0;

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               process[i],
               at[i],
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
