#include <stdio.h>
#include <omp.h>

#define N 30

int main()
{
    int speed[N] = {
        45, 72, 55, 81, 63, 50, 91, 58, 67, 49,
        74, 52, 88, 61, 43, 79, 56, 65, 70, 48,
        83, 59, 76, 54, 69, 47, 85, 62, 51, 73
    };

    int i;
    int total = 0;
    int alerts = 0;
    double average;
    double start, end;
    double serial_time, parallel_time;

    printf("Serial Version\n");

    start = omp_get_wtime();

    total = 0;
    alerts = 0;

    for (i = 0; i < N; i++)
    {
        total += speed[i];

        if (speed[i] > 60)
        {
            alerts++;
            printf("Alert: Vehicle %d speed = %d km/h\n", i + 1, speed[i]);
        }
    }

    average = (double)total / N;

    end = omp_get_wtime();

    serial_time = end - start;

    printf("\nTotal speed = %d km/h\n", total);
    printf("Average speed = %.2f km/h\n", average);
    printf("Number of alerts = %d\n", alerts);
    printf("Serial execution time = %f seconds\n", serial_time);

    printf("\nParallel Version\n");

    int thread_counts[] = {2, 4, 8};
    int num_counts = 3;

    for (int t = 0; t < num_counts; t++)
    {
        int threads = thread_counts[t];

        total = 0;
        alerts = 0;

        start = omp_get_wtime();

        #pragma omp parallel for reduction(+:total) num_threads(threads)
        for (i = 0; i < N; i++)
        {
            total += speed[i];

            if (speed[i] > 60)
            {
                #pragma omp critical
                {
                    printf("Alert: Vehicle %d speed = %d km/h\n",
                           i + 1, speed[i]);
                    alerts++;
                }
            }
        }

        end = omp_get_wtime();

        parallel_time = end - start;
        average = (double)total / N;

        double speedup = serial_time / parallel_time;
        double efficiency = speedup / threads;

        printf("\nThreads = %d\n", threads);
        printf("Total speed = %d\n", total);
        printf("Average speed = %.2f\n", average);
        printf("Number of alerts = %d\n", alerts);
        printf("Parallel execution time = %f seconds\n", parallel_time);
        printf("Speedup = %.2f\n", speedup);
        printf("Efficiency = %.2f\n", efficiency);
    }

    return 0;
}