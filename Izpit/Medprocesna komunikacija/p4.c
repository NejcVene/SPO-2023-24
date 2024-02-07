#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void sigPIPEHanler(int sigNum);

int main(void) {

    int fd;

    if (signal(SIGPIPE, sigPIPEHanler) == SIG_ERR) {
        err("signal", "cannot assing signal handler")
    }

    if (mkfifo("poimenovana_cev", 0644) < 0) {
        err("fifo", "cannot create FIFO")
    }

    if ((fd = open("poimenovana_cev", O_WRONLY)) < 0) {
        err("open", "cannot open FIFO")
    }

    if (write(fd, "Hello", strlen("Hello")) != strlen("Hello")) {
        err("write", "cannot write to FIFO")
    }

    if (close(fd) < 0) {
        err("close", "cannot close FIFO")
    }

}

void sigPIPEHanler(int sigNum) {

    printf("Got SIGPIPE with number %d\n", sigNum);

}