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

// Approach 2: threads double each level (1-2-4..), stops at NUM_THREADS
void* parallel_merge_sort(void* arg) {
    thread_data_t* d = (thread_data_t*)arg;
    int l = d->left, r = d->right;
    if (l >= r) return NULL;
    int m = (l + r) / 2;

    if ((2 << d->level) <= NUM_THREADS) {
        // new thread sorts left half, this thread sorts right half
        pthread_t t;
        thread_data_t left = {d->arr, l, m, d->level + 1};
        thread_data_t right = {d->arr, m + 1, r, d->level + 1};
        pthread_create(&t, NULL, parallel_merge_sort, &left);
        parallel_merge_sort(&right);
        pthread_join(t, NULL);
    } else {
        // thread limit reached, go sequential
        merge_sort_seq(d->arr, l, m);
        merge_sort_seq(d->arr, m + 1, r);
    }
    merge(d->arr, l, m, r);
    return NULL;
}

void merge_sort_p(int* arr, int l, int r) 
{
    pthread_t t;
    thread_data_t d = {arr, l, r, 0};
    pthread_create(&t, NULL, parallel_merge_sort, &d); // one top thread
    pthread_join(t, NULL);
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