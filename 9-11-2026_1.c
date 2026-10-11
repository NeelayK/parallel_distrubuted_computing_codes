#include<omp.h>
#include<stdio.h>

void do_work(int i){
    printf("Thread %d is doing work %d\n", omp_get_thread_num(), i);
}

int main(){
    int i=10;
    printf("Starting work with %d tasks\n", i);
    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int j=0; j<i; j++){
                #pragma omp task
                do_work(j);
            }
        }
    
    printf("Thread %d finished its work\n", omp_get_thread_num());
    
}
    printf("All threads have finished their work\n");
    return 0;

}