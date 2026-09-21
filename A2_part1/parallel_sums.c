// convert sequential sums to parallel

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESSES 4

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s <N> [num_processes]\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int P = (argc == 3) ? atoi(argv[2]) : NUM_PROCESSES;   // optional, for timing
    int *arr = malloc(N * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    int fd[P][2];              // one pipe per child: fd[i][0] = read, fd[i][1] = write
    int base = N / P;          // chunk size
    int rem = N % P;           // leftover elements go to the first few chunks

    for (int i = 0; i < P; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // Child: close read ends (its own and the earlier pipes it inherited)
            for (int j = 0; j <= i; j++) {
                close(fd[j][0]);
            }

            int start = i * base + (i < rem ? i : rem);
            int len = base + (i < rem ? 1 : 0);

            long long partial = 0;
            for (int k = start; k < start + len; k++) {
                partial += arr[k];
            }

            if (write(fd[i][1], &partial, sizeof(partial)) == -1) {
                perror("write");
                exit(1);
            }
            close(fd[i][1]);
            exit(0);
        }

        // Parent: doesn't write, so close the write end
        close(fd[i][1]);
    }

    // Wait for all children to finish
    for (int i = 0; i < P; i++) {
        wait(NULL);
    }

    // Collect each partial sum and add them up
    long long total = 0;
    for (int i = 0; i < P; i++) {
        long long partial = 0;
        if (read(fd[i][0], &partial, sizeof(partial)) == -1) {
            perror("read");
            exit(1);
        }
        close(fd[i][0]);
        total += partial;
    }

    printf("Total sum = %lld\n", total);
    free(arr);

    return 0;
}
