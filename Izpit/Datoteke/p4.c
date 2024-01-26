#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 10
#define err(msg) { perror(msg); exit(1); }

void readFile(int fd) {

    int readBytes;
    char buffer[BUFFER_SIZE];
    if ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("%s\n", buffer);
    }

    if (readBytes < 0) {
        err("read")
    }

}

void getCurrentSeek(int fd) {

    off_t offset;
    if ((offset = lseek(fd, 10, SEEK_CUR)) < 0) {
        err("lseek")
    }
    printf("Current seek: %ld\n", offset);

}

int main(void) {

    int fd;
    if ((fd = open("new_file.txt", O_RDONLY)) < 0) {
        err("open")
    }

    readFile(fd);
    getCurrentSeek(fd);
    readFile(fd);

    if (close(fd) < 0) {
        err("close")
    }

    return 0;

}