#include<stdio.h>
#include<omp.h>

int main(){
    int shared_int = 0;

    #pragma omp parallel sections
    {
        
        #pragma omp section
        {
            shared_int += 1;
            printf("Thread %d, shared_int=%d\n", omp_get_thread_num(), shared_int);
        }

        #pragma omp section
        {
            shared_int += 2;
            printf("Thread %d, shared_int=%d\n", omp_get_thread_num(), shared_int);
        }

        #pragma omp section
        {
            shared_int += 3;
            printf("Thread %d, shared_int=%d\n", omp_get_thread_num(), shared_int);
        }
    }
    printf("After parallel sections, shared_int=%d\n", shared_int);
    return 0;
}