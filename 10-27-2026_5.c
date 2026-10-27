#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 1000+rank;
    MPI_Win win;

    MPI_Win_create(&data, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    int recv_value =-1;
    MPI_Win_fence(0, win);

    if (rank == 0) {
        int val = 123;
        printf("Process 0: Writing %d to process 1\n", val);
        MPI_Get(&recv_value, 1, MPI_INT, 1, 0, 1, MPI_INT, win);
    }

    MPI_Win_fence(0, win);

    if (rank == 0) {
        printf("Process %d: received value = %d\n", rank, recv_value);
    }

    if(rank==1){
        printf("Process %d: Provided data =  %d\n",rank ,data);
    }

    MPI_Win_free(&win);
    MPI_Finalize();

    return 0;
}