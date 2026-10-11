#include<stdio.h>
#include<omp.h>

int main(){
    int N = 100;
    #pragma omp parallel if(N>50)
    {
        int thread_id = omp_get_thread_num();
        printf("Thread %d is executing.\n", thread_id);
    }
    return 0;

}