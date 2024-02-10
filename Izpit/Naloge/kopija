#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void signalHandler(int);
void signalHandlerSIGCHILD(int);

int main(void) {

    pid_t pid;
    int pfd[2],
        lineLength;
    char buffer[BUFFER_SIZE];

    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        err("signal", "cannot assing signal handler for SIGINT")
    }

    if (signal(SIGCHLD, signalHandlerSIGCHILD) == SIG_ERR) {
        err("signal", "cannot assing signal handler for SIGINT")
    }

    if (pipe(pfd) < 0) {
        err("pipe", "cannot create a pipe")
    }

    if ((pid = fork()) < 0) {
        err("vfork", "cannot create a process")
    } else if (pid == 0) {
        // child process

        close(pfd[1]);
        dup2(pfd[0], STDIN_FILENO);

        if (execl("/home/nejcv/FRI/SPO/SPO-2023-24/Izpit/Naloge/prog2", "prog2", (char *) 0) < 0) {
            err("execlp", "process cannot run prog2")
        }

        exit(0);
    } else {
        // parent process
        close(pfd[0]);

        while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            lineLength = strlen(buffer);
            if (write(pfd[1], buffer, lineLength) != lineLength) {
                err("write", "cannot write to pipe")
            }
        }
    }

    return 0;
}

void signalHandler(int sigNum) {

    printf("Vnesel si CTRL+C\n");

}

void signalHandlerSIGCHILD(int sigNum) {

    pid_t pid;
    int status;

    if ((pid = wait(&status)) < 0) {
        err("wait", "cannot wait for process")
    }
    printf("Child of PID %d exited with %d\n", pid, status);

}