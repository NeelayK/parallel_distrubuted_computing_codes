#include<stdio.h>
#include<omp.h>

int main(){
int s=0;
#pragma omp parallel for lastprivate(s)
    for(int i=0;i<4;i++)
    {
        s = i*2;
        printf("thread %d, s=%d\n", omp_get_thread_num(), s);
    }
printf("After parallel, s=%d\n", s);
return 0;
}