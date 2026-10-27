#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int local_val, global_sum;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    local_val = rank+1;

    MPI_Reduce( &local_val , &global_sum , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD);

    if (rank == 0) {
        printf("global sum=%d\n",global_sum);
        }
        MPI_Finalize();
        return 0;
    }