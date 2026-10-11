#include<stdio.h>
#include<omp.h>
#define N 8

int main(){

    printf("Started openmp nowait example...\n");
    #pragma omp parallel
    {
        #pragma omp for nowait
        for(int i=0;i<N;i++){
            printf("Loop 1: Thread %d is processing iteration %d\n", omp_get_thread_num(), i);
        }

        #pragma omp for
        for(int i=0;i<N;i++){
            printf("Loop 2: Thread %d is processing iteration %d\n", omp_get_thread_num(), i);
        }

    }

    printf("Finished openmp nowait example.\n");
    return 0;

}