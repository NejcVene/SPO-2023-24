#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    int fd, fdDup;
    if ((fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        err("open", "cannot open/create file.txt")
    }

    printf("Hello world\n");

    if ((fdDup = dup2(fd, STDOUT_FILENO)) < 0) {
        err("dup2", "cannot dup")
    }
    
    if (close(fd) < 0) {
        err("close", "cannot close file")
    }

    printf("Hello world\n");
    printf("Hello, the %s is asleep\n", "cat");

    return 0;

}