#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SIZE 100000000
#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

long long arr[SIZE];
long long partialSums[NUM_THREADS] = {0}; // Array to store partial sums for each thread

// Entry function for each thread
void* sumPart(void* arg)
{
    int id = *(int*)arg;
    int chunk = SIZE / NUM_THREADS;
    int start = id * chunk;
    int end = (id == NUM_THREADS - 1) ? SIZE : start + chunk; // last thread takes any leftover

    long long local = 0;
    for (int i = start; i < end; i++) {
        local += arr[i];
    }
    partialSums[id] = local; // each thread writes only its own slot
    return NULL;
}

int main()
{
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    // Combine the partial sums from all threads
    long long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += partialSums[i];
    }

    // Print the total sum;
    printf("Threads: %d\n", NUM_THREADS);
    printf("Total Sum: %lld\n", totalSum);
    printf("Time: %f seconds\n", (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9);

    return 0;
}
