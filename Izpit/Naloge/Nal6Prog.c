#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void signalHandler(int);

int main(void) {

    if (signal(SIGCHLD, signalHandler) == SIG_ERR) {
        err("signal", "cannot assing signal handler for SIGCHLD")
    }

    pid_t pid;
    int pfd[2];

    if (pipe(pfd) < 0) {
        err("pipe", "cannot create a pipe")
    }

    if ((pid = fork()) < 0) {
        err("fork", "cannot create process")
    } else if (pid > 0) {
        // parent process
        close(pfd[0]);
        char bufferParent[BUFFER_SIZE];
        int n;
        while (fgets(bufferParent, BUFFER_SIZE, stdin) != NULL) {
            n = strlen(bufferParent);
            if (write(pfd[1], bufferParent, n) != n) {
                err("write", "parent process cannot write to pipe")
            }
        }
        close(pfd[1]);

    } else {
        // child process
        close(pfd[1]);
        dup2(pfd[0], STDIN_FILENO);

        if (execlp("./Nal6FindMax", "./Nal6FindMax", (char *) 0) < 0) {
            err("execlp", "cannot run ./Nal6FindMax")
        }
        exit(0);

    }

    return 0;

}

void signalHandler(int sigNUM) {

    pid_t pid;
    int status;

    printf("Received signal: %d\n", sigNUM);

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("PID %d Status: %d\n", pid, status);
    }

    if (pid == -1 && errno != ECHILD) {
        err("It is not no child process(10)", "waitpid")
    }

}