#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    int fd;
    struct stat buffer, bufferFD;

    if ((fd = open("p1.c", O_RDONLY)) < 0) {
        err("open", "cold not open file")
    }

    if (stat("p1.c", &buffer) < 0) {
        err("stat", "could not stat file")
    }

    if (fstat(fd, &bufferFD) < 0) {
        err("fstat", "could not stat file")
    }

    if (S_ISREG(buffer.st_mode)) {
        printf("A regular file -> iNode %ld\n", buffer.st_ino);
    }

    if (S_ISREG(bufferFD.st_mode)) {
        printf("A regular file -> iNode %ld\n", bufferFD.st_ino);
    }

    if (close(fd) < 0) {
        err("close", "could not close file")
    }

    return 0;

}