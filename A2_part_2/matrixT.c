#include "matrixT.h"
#include <time.h>

int main()
{
    // declare thread id and thread data
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // Dynamically allocate memory for the matrices
    A = (int**)malloc(N * sizeof(int*));
    B = (int**)malloc(N * sizeof(int*));
    C = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Initialize matrices A and B with values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    // Create threads to perform matrix multiplication
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        // last thread also takes any leftover rows
        thread_data[i].num_rows = (i == NUM_THREADS - 1) ? N - i * (N / NUM_THREADS) : N / NUM_THREADS;
        pthread_create(&threads[i], NULL, matrixMultiplyThread, &thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    printf("Matrix multiplication complete!\n");

    // Check the result: every entry should equal N (since A and B are all 1s)
    int correct = 1;
    for (int i = 0; i < N && correct; ++i)
        for (int j = 0; j < N; ++j)
            if (C[i][j] != N) { correct = 0; break; }
    printf("Threads: %d\n", NUM_THREADS);
    printf("Result check: %s (C[0][0] = %d, C[%d][%d] = %d)\n", correct ? "CORRECT" : "WRONG", C[0][0], N-1, N-1, C[N-1][N-1]);
    printf("Time: %f seconds\n", (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9);

    // Optionally, display the resulting matrix C (Not when you are timing :) )
    // displayMatrix(C, N);

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
