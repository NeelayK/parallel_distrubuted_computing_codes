#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int data;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 4) {
        if (rank == 0) {
            printf("Please run with at least 4 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        data = 123;

        printf("Process %d sending data = %d to process 2\n",
               rank, data);

        MPI_Send(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        printf("Process %d done sending.\n", rank);
    }

    else if (rank == 1) {
        data = 456;

        printf("Process %d sending data = %d to process 3\n",
               rank, data);

        MPI_Send(&data, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);

        printf("Process %d done sending.\n", rank);
    }

    else if (rank == 2) {
        MPI_Recv(&data, 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received data = %d from process 0\n",
               rank, data);
    }

    else if (rank == 3) {
        MPI_Recv(&data, 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d received data = %d from process 1\n",
               rank, data);
    }

    MPI_Finalize();
    return 0;
}