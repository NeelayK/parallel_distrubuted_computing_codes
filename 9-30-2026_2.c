#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100000

void generate_lidar_data(float *lidar) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        lidar[i] = 50.0f + 5.0f * sinf(i * 0.1f) + (float)(rand() % 3);
    }
}

void generate_radar_data(float *radar) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        radar[i] = 60.0f + 3.0f * cosf(i * 0.02f) + (float)(rand() % 3);
    }
}

void generate_camera_data(float *camera) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        camera[i] = 100.0f + 10.0f * sinf(i * 0.005f) + (float)(rand() % 3);
    }
}

void fuse_sensors(float *lidar, float *radar, float *camera, float *fused) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        fused[i] = (0.45f * lidar[i] + 0.35f * radar[i] + 0.25f * camera[i]);
        fused[i] += 0.5f * sinf(fused[i] * 0.1f);
    }
}

int main(void) {
    float *lidar = (float *)malloc(N * sizeof(float));
    float *radar = (float *)malloc(N * sizeof(float));
    float *camera = (float *)malloc(N * sizeof(float));
    float *fused = (float *)malloc(N * sizeof(float));

    if (!lidar || !radar || !camera || !fused) {
        fprintf(stderr, "Memory allocation failed\n");
        free(lidar);
        free(radar);
        free(camera);
        free(fused);
        return 1;
    }

    srand((unsigned int)time(NULL));

    double baseline_time = 0.0;

    for (int p = 1; p <= 16; p *= 2) {
        omp_set_num_threads(p);

        double start_time = omp_get_wtime();
        generate_lidar_data(lidar);
        generate_radar_data(radar);
        generate_camera_data(camera);
        fuse_sensors(lidar, radar, camera, fused);
        double end_time = omp_get_wtime();

        double elapsed = end_time - start_time;

        printf("Threads: %d, Time taken: %.6f seconds\n", p, elapsed);

        if (p == 1) {
            baseline_time = elapsed;
        }

        if (elapsed > 0.0) {
            double speedup = baseline_time / elapsed;
            double efficiency = speedup / p;

            printf("Speedup: %.6f\n", speedup);
            printf("Efficiency: %.6f\n", efficiency);
        } else {
            printf("Speedup: N/A\n");
            printf("Efficiency: N/A\n");
        }
    }

    free(lidar);
    free(radar);
    free(camera);
    free(fused);

    return 0;
}