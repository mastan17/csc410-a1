#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000000000 // intervals
#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// use this shared global variable
double total_sum = 0.0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *parallel_trapezoidalRule(void *arg) 
{
    long id = (long)arg;
    double h = 1.0 / N;
    long chunk = N / NUM_THREADS;
    long start = (id == 0) ? 1 : id * chunk;
    long end = (id == NUM_THREADS - 1) ? N : (id + 1) * chunk;

    double local = 0.0;
    for (long i = start; i < end; i++)
        local += f(i * h);

    // add this thread's part to the shared sum
    pthread_mutex_lock(&lock);
    total_sum += local * h;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    pthread_t threads[NUM_THREADS];
    struct timespec s, e;
    double h = 1.0 / N;

    clock_gettime(CLOCK_MONOTONIC, &s);
    total_sum = (f(0.0) + f(1.0)) / 2.0 * h; // endpoints

    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, parallel_trapezoidalRule, (void *)i);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &e);
    pthread_mutex_destroy(&lock);

    printf("Result of numerical integration: %f\n", total_sum);
    printf("Threads: %d  Time: %f s\n", NUM_THREADS,
           (e.tv_sec - s.tv_sec) + (e.tv_nsec - s.tv_nsec) / 1e9);

    return 0;
}
