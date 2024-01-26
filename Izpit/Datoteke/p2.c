#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256
#define err(msg) { perror(msg); exit(1); }

int main(void) {

    int fd, fd2;
    int readBytes;
    char buffer[BUFFER_SIZE];
    if ((fd = open("new_file.txt", O_RDONLY)) < 0) {
        err("open")
    }

    if ((fd2 = open("new_file2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        err("open")
    }

    while((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("%d\n", readBytes);
        if (write(fd2, buffer, readBytes) != readBytes) {
            err("write")
        }
    }

    if (close(fd) < 0) {
        err("close")
    }

    if (close(fd2) < 0) {
        err("close")
    }

    return 0;

}