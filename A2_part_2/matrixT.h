#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Size of the matrix
#ifndef NUM_THREADS
#define NUM_THREADS 4  // Number of threads
#endif

int **A, **B, **C;  // Global matrices

// Structure to hold information for each thread
typedef struct
{
    int thread_id;
    int num_rows;  // Number of rows each thread will handle
} thread_data_t;

// Function for each thread to perform matrix multiplication
void* matrixMultiplyThread(void* arg)
{
    thread_data_t* data = (thread_data_t*)arg;
    int start = data->thread_id * (N / NUM_THREADS);  // first row for this thread
    int end = start + data->num_rows;                 // one past the last row

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;  // each thread writes only its own rows
        }
    }
    return NULL;
}

void displayMatrix(int** matrix, int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
