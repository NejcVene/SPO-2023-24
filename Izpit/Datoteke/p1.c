#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define err(msg) { perror(msg); exit(1); }

void createFile(char *);

int main(void) {

    int fd;
    if ((fd = open("p1_test.c", O_RDONLY | O_DIRECTORY)) < 0) {
        err("open")
    }
    printf("File desc. is %d\n", fd);

    if (close(fd) < 0) {
        err("fclose")
    }

    createFile("new_file.txt");

}

void createFile(char *filename) {

    int fd;
    if ((fd = open(filename, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0777)) < 0) {
        err("open")
    }

    if (close(fd) < 0) {
        err("close")
    }

}