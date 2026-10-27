#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int data;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data_send, data_recv;
    MPI_Request request_send, request_recv;
    MPI_Status status;

    if (rank == 0) {
        data = 123;

        printf("Process %d sending data = %d \n",
               rank, data_send);
            MPI_ISend(&data_send, 1, MPI_INT, 1, 99, MPI_COMM_WORLD, &request_send);
            printf("Process %d done sending.\n", rank);
            MPI_Wait(&request_send , &status);

        }

    }