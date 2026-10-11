#include<stdio.h>
#include<omp.h>

int main()
{
int a=10;
#pragma omp parallel firstprivate(a)
{
    int tid = omp_get_thread_num();
    a += tid+1;
    printf("thread %d, a=%d\n", tid, a);
}
printf("After parallel, a=%d\n", a);
return 0;
}