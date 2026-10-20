#include "p_merge.h"

void merge(int* arr, int l, int m, int r) 
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_seq(int* arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Approach 1: split array into NUM_THREADS segments, each thread sorts one
void* parallel_merge_sort(void* arg) {
    thread_data_t* d = (thread_data_t*)arg;
    merge_sort_seq(d->arr, d->left, d->right);
    return NULL;
}

void merge_sort_p(int* arr, int l, int r) 
{
    pthread_t t[NUM_THREADS];
    thread_data_t d[NUM_THREADS];
    int b[NUM_THREADS + 1]; // segment boundaries
    int n = r - l + 1;

    for (int i = 0; i <= NUM_THREADS; i++)
        b[i] = l + (long)i * n / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        d[i].arr = arr;
        d[i].left = b[i];
        d[i].right = b[i + 1] - 1;
        d[i].level = 0;
        pthread_create(&t[i], NULL, parallel_merge_sort, &d[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL);

    // merge sorted segments one by one
    for (int i = 1; i < NUM_THREADS; i++)
        merge(arr, l, b[i] - 1, b[i + 1] - 1);
}


void printArray(int* arr, int n) 
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

double time_diff_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}