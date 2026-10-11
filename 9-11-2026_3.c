#include<omp.h>
#include<stdio.h>

int main(){
    int x =5, y = 10;
    #pragma omp parallel private(y)
    {
        for(int i = 0 ; i<8 ; i++){
            #pragma omp order
               int tid=omp_get_thread_num();
               y=tid*2;
               printf("Thread %d: x=%d, y=%d\n", tid, x, y);

        }
    }
    printf("Outside parallel region: x=%d, y=%d\n", x, y);
    return 0;
}