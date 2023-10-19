#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

#define BUFFER_SET_POSITION_SIZE 1
#define BUFFER_SIZE              256

int readFile(int, char *, int, int*);
void writeFile(int, char *, int);
void moveCursorPosition(int, char *);

int main(int argc, char **argv) {

    int fd, fd2, readBytes, delLineCount = atoi(argv[2]), n = 1, newOffset, c = 0, sum = 0;
    printf("To delete: %d\n", delLineCount);
    bool lineFound = false;
    char buffer[BUFFER_SIZE], bufferSetPos[BUFFER_SET_POSITION_SIZE];
    if ((fd = open(argv[3], O_RDONLY)) < 0 || (fd2 = open("tmp", O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0) {
        err("Could not open source file")
    }
    while (1) {
        c = readFile(fd, buffer, delLineCount, &n);
        sum += c;
        if (c == 0) {
            break;
        }
        writeFile(fd2, buffer, c);
        // printf("!%d!\n!n = %d!\n", c, n);
        for (int i = 0; i<c; i++) {
            printf("%c", buffer[i]);
        }
    }
    int offset = lseek(fd, 0, SEEK_CUR);
    // printf("Offset: %d\n", offset);
    offset = lseek(fd, sum, SEEK_SET);
    // printf("Offset: %d\n", offset);
    moveCursorPosition(fd, bufferSetPos);
    // printf("SUM: %d\nSUM total: %d\n", sum, sum + delLineCount);
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            printf("%c", buffer[i]);
        }
        writeFile(fd2, buffer, readBytes);
    }
    if (unlink(argv[3]) == -1) {
        err("Could not unlink")
    }
    if (rename("tmp", argv[3])) {
        err("Could not rename")
    }
    /*
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            printf("%c", buffer[i]);
        }
    }
    */
    /*
    printf("File descriptor: %d\n", fd);
    c = readFile(fd, buffer, delLineCount);
    printf("!%d!\n", c);
    for (int i = 0; i<c; i++) {
        printf("%c", buffer[i]);
    }
    */
    // writeFile(fd2, buffer, readFile(fd, buffer, delLineCount));
    /*
    moveCursorPosition(fd, bufferSetPos);
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        writeFile(fd2, buffer, readBytes);
    }
    */
    if (close(fd) < 0 || close(fd2) < 0) {
        err("Could not close source file")
    }
    return 0;
    /*
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            c++;
            if (buffer[i] == '\n') {
                n++;
                if (n == delLineCount) {
                    printf("Line found\n");
                    printf("!%d!\n", c);
                    if (write(fd2, buffer, c) != c) {
                        err("Write failed")
                    }
                    lineFound = true;
                }
            }
            printf("%c", buffer[i]);
        }
        if (lineFound) {
            break;
        }
        printf("\n");
    }
    */
    printf("N: %d\n", n);
    // read head premakni naprej do naslednjena \n
    while (bufferSetPos[0] != '\n') {
        if ((readBytes = read(fd, bufferSetPos, BUFFER_SET_POSITION_SIZE)) == -1) {
            err("Cound not read")
        }
    }
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n");
        if (write(fd2, buffer, readBytes) != readBytes) {
            err("Could not write a second time")
        }
    }
    if (close(fd) < 0 || close(fd2) < 0) {
        err("Could not close source file")
    }

    return 0;

}

int readFile(int fd, char *buffer, int delLineCount, int *pN) {

    // printf("N is: %d\n", *pN);
    if ((*pN) == delLineCount) {
        // printf("END\n");
        return 0;
    }
    int readBytes, c = 0;
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            // printf("%c", buffer[i]);
            c++;
            if (buffer[i] == '\n') {
                (*pN)++;
                if ((*pN) == delLineCount) {
                    // printf("LINE FOUND\n");
                    return c;
                }
            }
        }
        if (readBytes == BUFFER_SIZE) {
            // printf("BUFFER FULL\n");
            return c;
        }
    }

    // printf("HERE and n is %d\n", *pN);
    return c;

}

void writeFile(int fd, char *buffer, int bytesToWrite) {

    if (write(fd, buffer, bytesToWrite) != bytesToWrite) {
        err("Could now write in function")
    }

}

void moveCursorPosition(int fd, char *buffer) {

    int readBytes;
    while (buffer[0] != '\n') {
        if ((readBytes = read(fd, buffer, BUFFER_SET_POSITION_SIZE)) == -1) {
            err("Cound not read")
        }
    }

}

/*
while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("Read bytes1: %d\n", readBytes);
        for (int i = 0; i<readBytes; i++) {
            if (buffer[i] == '\n') {
                n++;
                if (n == delLineCount) {
                    printf("Line found\n");
                    lineFound = true;
                    break;
                }
            }
        }
        if (lineFound) {
            break;
        }
    }*/