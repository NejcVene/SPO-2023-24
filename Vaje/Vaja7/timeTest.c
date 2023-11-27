#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

time_t start_time; // Global variable to store the start time in seconds

void printStartTime() {
    time_t current_time;
    time(&current_time);

    time_t elapsed_time = current_time - start_time;

    printf("Process started at: %ld seconds\n", elapsed_time);
}

void printElapsedTime() {
    time_t current_time;
    time(&current_time);

    time_t elapsed_time = current_time - start_time;

    printf("Elapsed time: %ld seconds\n", elapsed_time);
}

int main() {
    int N = 5; // Number of processes to create

    time(&start_time); // Record the start time in the main process

    for (int i = 0; i < N; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            printStartTime(); // Print the relative start time for each child
            printf("Child process %d (PID %d)\n", i + 1, getpid());
            // Add your child process logic here
            sleep(5);
            printElapsedTime(); // Print the elapsed time before exiting

            exit(EXIT_SUCCESS); // Terminate child process
        }
    }

    // Parent process
    for (int i = 0; i < N; ++i) {
        int status;
        pid_t child_pid = waitpid(-1, &status, 0); // Wait for any child process to finish
        if (WIFEXITED(status)) {
            printf("Child process %d (PID %d) exited with status %d\n", i + 1, child_pid, WEXITSTATUS(status));
        } else {
            printf("Child process %d (PID %d) did not exit normally\n", i + 1, child_pid);
        }
    }

    return 0;
}
