#include <stdio.h>
#include <omp.h>

#define N 200

void print_matrix(float matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrix_multiply_and_normalize(int A[N][N], int B[N][N], float C[N][N], int con) {
    #pragma omp parallel if(con > 2)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        for (int i = tid; i < N; i += num_threads) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        #pragma omp barrier

        for (int i = tid; i < N; i += num_threads) {
            int max_value = C[i][0];

            for (int j = 1; j < N; j++) {
                if (C[i][j] > max_value) {
                    max_value = C[i][j];
                }
            }

            for (int j = 0; j < N; j++) {
                C[i][j] = C[i][j] / max_value;
            }

            //printf("Thread %d: Maximum value in row %d = %d\n",tid, i, max_value);
        }

        #pragma omp barrier
    }
}

int main() {
    int A[N][N], B[N][N];
    float C[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j + 1;
            B[i][j] = (i == j) ? 1 : 0;  // Identity matrix
            C[i][j] = 0;
        }
    }

    double start_time = omp_get_wtime();
    matrix_multiply_and_normalize(A, B, C, 1);

    printf("\nMatrix C after multiplication and normalization:\n");
    //print_matrix(C);

    double end_time = omp_get_wtime();

    for(int p=2;p<=8;p+=2){
        omp_set_num_threads(p);
        double start_time2 = omp_get_wtime();
        matrix_multiply_and_normalize(A, B, C, p);

        printf("\nMatrix C after multiplication and normalization with %d threads:\n", p);
        //print_matrix(C);

        double end_time2 = omp_get_wtime();
        double serial_time = end_time - start_time;
        double parallel_time = end_time2 - start_time2;
        
        printf("Serial execution time: %.8f seconds\n", serial_time);
        printf("Parallel execution time: %.8f seconds\n", parallel_time);
        printf("Speedup: %.2f\n", serial_time / parallel_time);
        printf("Efficiency: %.2f%%\n", (serial_time / parallel_time) / p * 100);
    }
    return 0;
}