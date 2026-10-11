#include<stdio.h>
#include<omp.h>

int main(){
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Print before barrier %d\n", tid);
    #pragma omp barrier
    printf("Print after barrier %d\n", tid);
    }

    return 0;
}