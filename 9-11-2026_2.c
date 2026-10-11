#include<omp.h>
#include<stdio.h>


int main(){
int shared_var=0;
int i;
#pragma omp parallel private(i)
{
    int tid = omp_get_thread_num();
    printf("Thread %d incremented shared_var to %d\n", tid, shared_var);
        }
    }