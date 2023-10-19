#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

#define BUFFER_SET_POSITION_SIZE 1
#define BUFFER_READ_SIZE        256

int main(int argc, char **argv) {

    int fd, n, readBytes, newOffset;
    char bufferSetPos[BUFFER_SET_POSITION_SIZE], bufferRead[BUFFER_READ_SIZE];
    switch (argc) {
        case 2:
            n = 5;
            if ((fd = open(argv[1], O_RDONLY)) < 0) {
                err("Could not open source file")
            }
            break;
        case 4:
            if (strcmp(argv[1], "-n") != 0) {
                err("Unknow flag")
            }
            n = atoi(argv[2]);
            if ((fd = open(argv[3], O_RDONLY)) < 0) {
                err("Could not open source file")
            }
            break;
        default:
            err("Wrong arguments")
            break;
    }
    if ((newOffset = lseek(fd, 0, SEEK_END)) == -1) {
        err("Could not seek to end of file")
    }
    while (n > 0) {
        if ((newOffset = lseek(fd, -2, SEEK_CUR)) == -1) {
            err("Could not seek file")
        }
        if ((readBytes = read(fd, bufferSetPos, BUFFER_SET_POSITION_SIZE)) == -1) {
            err("Could not read")
        }
        if (bufferSetPos[0] == '\n') {
            n--;
        }
    }
    while ((readBytes = read(fd, bufferRead, BUFFER_READ_SIZE)) > 0) {
        for (int i = 0; i<readBytes; i++) {
            printf("%c", bufferRead[i]);
        }
    }
    if (close(fd) < 0) {
        err("Could not close source file")
    }

    return 0;

}
