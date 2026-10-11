#include<stdio.h>
#include<omp.h>

int main(){
    int sum =0 ;
    #pragma omp parallel
    {
    #pragma omp critical
        {
        sum +=1;
        printf("Thread %d, sum=%d\n", omp_get_thread_num(), sum);
    }}

    return 0;
}