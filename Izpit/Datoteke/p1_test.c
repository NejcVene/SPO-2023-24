#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define err(msg) { perror(msg); exit(1); }

void createFile(char *);

int main(void) {

    int fd;
    if ((fd = open("p1.c", O_RDONLY | O_WRONLY)) < 0) {
        err("open")
    }
    printf("File desc. is %d\n", fd);

    if (close(fd) < 0) {
        err("fclose")
    }

}

void createFile(char *filename) {

    int fd;
    if ((fd = open(filename, O_CREAT, 0777)) < 0) {
        err("open")
    }

    if (close(fd) < 0) {
        err("close")
    }

}