#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void sigpipeHandler(int sigNum) {

    printf("Got signal %d\n", sigNum);

}

int main(void) {

    signal(SIGPIPE, sigpipeHandler);

    int exitStatus;
    int pfd[2];
    pid_t pid;
    char string[] = "Hello, pipe!";
    char buffer[strlen(string)];

    if (pipe(pfd) < 0) {
        err("pipe", "cannot create a pipe")
    }

    if ((pid = fork()) < 0) {
        err("pid", "cannot create a child")
    } else if (pid == 0) {
        // child program
        printf("Child with PID: %d\n", getpid());
        close(pfd[0]);
        // close(pfd[1]);

        // read on pipe from parent
        read(pfd[0], buffer, strlen(string));
        printf("Pipe says: %s\n", buffer);

        exit(0);
    } else {
        // parent program
        close(pfd[0]);

        // write on pipe to child
        if (write(pfd[1], string, strlen(string)) != strlen(string)) {
            exit(1);
        }

        wait(&exitStatus);
        printf("Child %d exit with %d\n", pid, exitStatus);
    }

    return 0;

}