#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 10
#define err(msg) { perror(msg); exit(1); }

void writeToFile(int fd) {

    char buffer[] = "Hello";
    if (write(fd, buffer, strlen(buffer)) != strlen(buffer)) {
        err("write")
    }

}

int main(void) {

    int fd, newFd, newFd2 = 10, newFd3 = 1;
    if ((fd = open("new_file.txt", O_WRONLY | O_APPEND)) < 0) {
        err("open")
    }

    if ((newFd = dup(fd)) < 0) {
        err("dup")
    }

    if ((newFd2 = dup2(fd, newFd2)) < 0) {
        err("dup2")
    }

    if ((newFd3 = dup2(fd, newFd3)) < 0) {
        err("dup2")
    }
    printf("dup2: %d\n", newFd2);
    printf("dup2 (3): %d\n", newFd3);

    writeToFile(fd);

    writeToFile(newFd);

    writeToFile(newFd2);

    printf("Mjav\n");

    if (close(fd) < 0) {
        err("close")
    }

}