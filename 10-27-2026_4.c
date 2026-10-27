#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data = 0;
    MPI_Win win;

    MPI_Win_create(&data, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank == 0) {
        int val = 123;
        printf("Process 0: Writing %d to process 1\n", val);
        MPI_Put(&val, 1, MPI_INT, 1, 0, 1, MPI_INT, win);
    }

    MPI_Win_fence(0, win);

    if (rank == 1) {
        printf("Process 1: received value = %d\n", data);
    }

    MPI_Win_free(&win);
    MPI_Finalize();

    return 0;
}