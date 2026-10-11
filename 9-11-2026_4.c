#include<omp.h>
#include<stdio.h>


int main(){
int a=2,b=3;
int s=0;
#pragma omp parallel num_threads(2)
{
    int tid = omp_get_thread_num();
    s=s+a*b;
    printf("Thread %d computed S=%d\n", tid, s);
}
printf("Final value of S=%d\n", s);
return 0;   

}