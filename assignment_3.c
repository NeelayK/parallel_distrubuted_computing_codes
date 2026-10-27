#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 512        //// image size
#define OFFSET 40   // brightness increasing offset

void generateImage(char image[N][N]){
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            image[i][j] = (i + j) % 256;
        }}
}

void brightnessSerial(char image[N][N], char result[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int value = image[i][j] + OFFSET;

            if (value > 255)
                value = 255;

            result[i][j] = value;
}}}

void brightnessParallel(char image[N][N],
                        char result[N][N])
{
    #pragma omp parallel for
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int value = image[i][j] + OFFSET;

            if (value > 255)
                value = 255;

            result[i][j] = value;
        }}
}

void filterSerial(char image[N][N],char result[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            int sum = 0;
            int count = 0;

            for (int x = -1; x <= 1; x++){
                for (int y = -1; y <= 1; y++)
                {
                    int a = i + x;
                    int b = j + y;

                    if (a >= 0 && a < N && b >= 0 && b < N)
                    {
                        sum = sum + image[a][b];
                        count++;
                    }
                }
            }
            result[i][j] = sum / count;
        }}}

void filterParallel(char image[N][N],
                    char result[N][N])
{
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0;
            int count = 0;

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int a = i + x;
                    int b = j + y;

                    if (a >= 0 && a < N && b >= 0 && b < N)
                    {
                        sum = sum + image[a][b];
                        count++;
                    }}
            }
            result[i][j] = sum / count;
        }
    }
}

int main()
{
    char image[N][N];
    char bright[N][N];
    char output[N][N];

    char bright2[N][N];
    char output2[N][N];

    generateImage(image);

    double start, end;
    double serialTime;
    double parallelTime;

    start = omp_get_wtime();

    brightnessSerial(image, bright);
    filterSerial(bright, output);

    end = omp_get_wtime();

    serialTime = end - start;

    printf("Serial time: %f seconds\n", serialTime);

    int threads[] = {1, 2,4,8,16,32,64};
    int n = 7;

    printf("\nThreads\tTime\t\tSpeedup\t\tEfficiency\n");

    for (int k = 0; k < n; k++)
    {
        omp_set_num_threads(threads[k]);

        start = omp_get_wtime();

        brightnessParallel(image, bright2);
        filterParallel(bright2, output2);

        end = omp_get_wtime();

        parallelTime = end - start;

        double speedup = serialTime / parallelTime;
        double efficiency = (speedup / threads[k]) * 100;

        printf("%d\t%f\t%f\t%f%%\n",
               threads[k],
               parallelTime,
               speedup,
               efficiency);
    }

    int error = 0;

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (output[i][j] != output2[i][j])
            {
                error++;
            }
        }
    }

    if (error == 0)
        printf("\nResults are same\n");
    else
        printf("\nResults are different\n");

    return 0;
}