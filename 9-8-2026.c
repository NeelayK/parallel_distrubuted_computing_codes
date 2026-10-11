#include<stdio.h>
#include<omp.h>

int main()
{
    printf("Program starts in sequential mode\n");

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from thread %d\n", tid);
    }


    printf("Program ends in sequential mode\n");
    return 0;
}

