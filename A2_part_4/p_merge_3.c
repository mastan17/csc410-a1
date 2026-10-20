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

// Approach 3: fixed thread pool, tasks (sort or merge) go in a queue
typedef struct { int l, m, r; } task_t; // m == -1 means sort task

int* A;
task_t q[64];
int qh = 0, qt = 0, pending = 0, stop = 0;
pthread_mutex_t lock;
pthread_cond_t has_work, all_done;

void add_task(int l, int m, int r) {
    pthread_mutex_lock(&lock);
    q[qt].l = l; q[qt].m = m; q[qt].r = r;
    qt++;
    pending++;
    pthread_cond_signal(&has_work);
    pthread_mutex_unlock(&lock);
}

void wait_all() {
    pthread_mutex_lock(&lock);
    while (pending > 0)
        pthread_cond_wait(&all_done, &lock);
    pthread_mutex_unlock(&lock);
}

void* parallel_merge_sort(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (qh == qt && !stop)
            pthread_cond_wait(&has_work, &lock);
        if (qh == qt && stop) {
            pthread_mutex_unlock(&lock);
            break;
        }
        task_t t = q[qh++];
        pthread_mutex_unlock(&lock);

        if (t.m < 0) merge_sort_seq(A, t.l, t.r);
        else merge(A, t.l, t.m, t.r);

        pthread_mutex_lock(&lock);
        if (--pending == 0)
            pthread_cond_broadcast(&all_done);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void merge_sort_p(int* arr, int l, int r) 
{
    pthread_t t[NUM_THREADS];
    int b[NUM_THREADS + 1];
    int n = NUM_THREADS;
    A = arr;
    qh = qt = pending = stop = 0;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&has_work, NULL);
    pthread_cond_init(&all_done, NULL);

    // create the pool
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&t[i], NULL, parallel_merge_sort, NULL);

    // sort tasks, one per segment
    for (int i = 0; i <= n; i++)
        b[i] = l + (long)i * (r - l + 1) / n;
    for (int i = 0; i < n; i++)
        add_task(b[i], -1, b[i + 1] - 1);
    wait_all();

    // merge tasks, pairs of segments each round
    while (n > 1) {
        for (int i = 0; i + 1 < n; i += 2)
            add_task(b[i], b[i + 1] - 1, b[i + 2] - 1);
        wait_all();
        int j = 0;
        for (int i = 0; i <= n; i += 2) b[j++] = b[i];
        if (n % 2) b[j++] = b[n];
        n = j - 1;
    }

    // shut down the pool
    pthread_mutex_lock(&lock);
    stop = 1;
    pthread_cond_broadcast(&has_work);
    pthread_mutex_unlock(&lock);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&has_work);
    pthread_cond_destroy(&all_done);
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