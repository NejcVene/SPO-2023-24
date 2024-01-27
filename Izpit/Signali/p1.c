#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void saySIGINT(int sigNum) {

    printf("I've got %d\n", sigNum);

}

int main(void) {

    if (signal(SIGINT, saySIGINT) == SIG_ERR) {
        err("signal", "cannot change signal handler")
    }

    // you have to kill this program using: kill -9 PID
    while (1) {
        if (raise(SIGINT) < 0) {
            err("raise", "cannot raise signal SIGINT to myself")
        }
        sleep(3);
    }

    return 0;

}