#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 10
#define err(msg) { perror(msg); exit(1); }

int main(void) {

    // copy from STDIN_FILENO to STDOUT_FILENO
    // no need for open, as these two are fd's already

    int readBytes;
    char buffer[BUFFER_SIZE];

    while ((readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, readBytes) != readBytes) {
            err("write")
        }
    }

    if (readBytes < 0) {
        err("read")
    }

    return 0;

}