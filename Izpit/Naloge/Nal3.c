#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void readThreeLines(char *);

int main(void) {

    readThreeLines("prog1.c");

    return 0;

}

void readThreeLines(char *filename) {

    int fd, lineCount = 0, readBytes, chCount, r = 0;
    char buffer[BUFFER_SIZE];
    if ((fd = open(filename, O_RDONLY)) < 0) {
        err("open", "cannot open specified file")
    }

    if ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (chCount = 0; chCount<readBytes; chCount++) {
            if (buffer[chCount] == '\n') {
                lineCount++;
                if (lineCount == 3) {
                    r = chCount;
                }
            }
        }
        if (lineCount == 3) {
            if (write(STDOUT_FILENO, buffer, chCount) != readBytes) {
                err("write", "cannot write to STDOUT")
            }
            printf("\n");
        } else if (lineCount < 3) {
            ;
        } else {
            ;
        }
    }

    if (close(fd) < 0) {
        err("close", "cannot close specified file")
    }

}