#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100000

void generate_lidar_data(float *lidar) {
    for (int i = 0; i < N; i++) {
        lidar[i] = 50.0f + 5.0f * sinf(i * 0.1f) + (float)(rand() % 3);
    }
}

void generate_radar_data(float *radar) {
    for (int i = 0; i < N; i++) {
        radar[i] = 60.0f + 3.0f * cosf(i * 0.02f) + (float)(rand() % 3);
    }
}

void generate_camera_data(float *camera) {
    for (int i = 0; i < N; i++) {
        camera[i] = 100.0f + 10.0f * sinf(i * 0.005f) + (float)(rand() % 3);
    }
}

void fuse_cpu(const float *lidar, const float *radar, const float *camera,
              float *fused) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        fused[i] = (0.45f * lidar[i] + 0.35f * radar[i] + 0.25f * camera[i]);
        fused[i] += 0.5f * sinf(fused[i] * 0.1f);
    }
}

void fuse_gpu_simulated(const float *lidar, const float *radar,
                        const float *camera, float *fused) {
    #pragma omp parallel for schedule(dynamic, 1)
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

    generate_lidar_data(lidar);
    generate_radar_data(radar);
    generate_camera_data(camera);

    for (int p = 1; p <= 16; p *= 2) {
        omp_set_num_threads(p);

        double start_time = omp_get_wtime();
        fuse_cpu(lidar, radar, camera, fused);
        double cpu_time = omp_get_wtime() - start_time;

        start_time = omp_get_wtime();
        fuse_gpu_simulated(lidar, radar, camera, fused);
        double gpu_simulated_time = omp_get_wtime() - start_time;

        printf("Threads: %d, CPU (static) time: %.6f seconds, "
               "GPU-simulated (dynamic,1) time: %.6f seconds\n",
               p, cpu_time, gpu_simulated_time);
        printf("Speedup: %.2f\n", gpu_simulated_time/cpu_time);
    }

    free(lidar);
    free(radar);
    free(camera);
    free(fused);

    return 0;
}