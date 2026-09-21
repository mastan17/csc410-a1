#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;

int main() {
    pid_t pid;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);
    fflush(stdout);   // flush before forking so the children don't repeat this line

    // Each character and how much they adjust the shield power (in percent)
    const char *names[] = {"Luke", "Han", "Chewbacca", "Leia"};
    const int adjust[]  = {25,     20,    30,          15};

    // Create 4 child processes - 4 different characters adjusting shield power
    for (int i = 0; i < 4; i++) {
        pid = fork();

        // Check if process creation failed
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Child process: only changes this child's own copy of shield_power
            printf("%s: Adjusting shields...\n", names[i]);
            shield_power += adjust[i];
            printf("%s: Shield power level now at %d%%\n", names[i], shield_power);
            exit(0);
        }
    }

    // Make parent process wait for all child processes to complete
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
