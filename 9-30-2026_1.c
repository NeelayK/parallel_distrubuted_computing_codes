#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

#define FRAMES 1000
#define SECTORS 360

int main(){
    static float lidar_dist[FRAMES][SECTORS];
    static float radar_dist[FRAMES][SECTORS];
    static float fused_dist[FRAMES][SECTORS];
    double local_sum = 0.0;
    omp_set_num_threads(8);
    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        #pragma omp for collapse(2) schedule(dynamic)
        for (int i = 0; i < FRAMES; i++) {
            for (int j = 0; j < SECTORS; j++) {
                lidar_dist[i][j] = 50.0f +(float)(rand() % 100);
                radar_dist[i][j] = 48.0f +(float)(rand() % 100);
            }
        }
    #pragma omp single
    {
        printf("Data generation completed by thread %d\n", tid);
    }
    #pragma omp barrier

        #pragma omp for collapse(2) schedule(static)
        for (int i = 0; i < FRAMES; i++) {
            for (int j = 0; j < SECTORS; j++) {
                fused_dist[i][j] = (lidar_dist[i][j] < radar_dist[i][j] ? lidar_dist[i][j] : radar_dist[i][j]);
            }
        }

    #pragma omp for collapse(2) reduction(+:local_sum)
        for (int i = 0; i < FRAMES; i++) {
            for (int j = 0; j < SECTORS; j++) {
                local_sum += fused_dist[i][j];
            }
        }
        #pragma omp single
        {
            double avg = local_sum / (FRAMES * SECTORS);
            printf("Average fused distance: %.2f\n", avg);
        }
        #pragma omp for
        for(int f=0;f<3;f++){
            #pragma omp critical
            {
                printf("Thread %d: Processing frame %d\n", tid, f);
                for(int s=0;s<SECTORS;s++){
                    //printf("Thread %d: Frame %d, Sector %d, Fused Distance: %.2f\n", tid, f, s, fused_dist[f][s]);
                    printf("");
                }
                printf("\n\n");
            }
        }
    }
    double end = omp_get_wtime();
    printf("Execution time: %.2f seconds\n", end - start_time);
    return 0;
}