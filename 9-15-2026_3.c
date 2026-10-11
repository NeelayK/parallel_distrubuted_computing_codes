#include<stdio.h>
#include<omp.h>

static int counter = 0;
#pragma omp threadprivate(counter)  // if you comment this then counter will be equal to the last thread number in the firt loop
int main(){
    printf("\n-------------------------------------\n");
    printf("Parallel with counter, counter=%d\n", counter);
    printf("-------------------------------------\n");
    #pragma omp parallel
    {
        counter=omp_get_thread_num();
        printf("Thread %d, counter=%d\n", omp_get_thread_num(), counter);
    }
    printf("\n-------------------------------------\n");
    printf("Parallel without counter, counter=%d\n", counter);
    printf("-------------------------------------\n");
    #pragma omp parallel
    {
        printf("Thread %d, counter=%d\n", omp_get_thread_num(), counter);
    }
    return 0;
}