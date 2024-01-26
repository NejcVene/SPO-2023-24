#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    int fd, readBytes;
    char buffer[BUFFER_SIZE];

    switch (argc) {
        case 2:
            if ((fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0) {
                err("open", "could not open dest file")
            }
            break;
        case 3:
            if (strcmp(argv[1], "-a") == 0 && (fd = open(argv[2], O_WRONLY | O_APPEND)) < 0) {
                err("open", "could not open dest file or wrong argument")
            }
            break;
        default:
            err("Wrong usage", "Usage: (-a) <dst>")
            break;
    }

    while ((readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, readBytes) != readBytes) {
            err("write", "could not write STDOUT")
        }
        if (write(fd, buffer, readBytes) != readBytes) {
            err("write", "could not write to destination file")
        }
    }

    if (close(fd) < 0) {
        err("close", "could not close file")
    }
 
    return 0;

}