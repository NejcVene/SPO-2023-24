#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 128
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void closeFile(int);
char *createNewFileName(char *);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <src> <dst>")
    }

    int src, dst, readBytes;
    char *newFileName;
    char buffer[BUFFER_SIZE];

    if ((src = open(argv[1], O_RDONLY)) < 0) {
        err("open", "could not open source file")
    }

    newFileName = createNewFileName(argv[1]);
    printf("New file name: %s\n", newFileName);

    if ((dst = open(newFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        err("open", "could not open/create destination file")
    }


    while ((readBytes = read(src, buffer, BUFFER_SIZE)) > 0) {
        if (write(dst, buffer, readBytes) != readBytes) {
            err("write", "could not write to destination file")
        }
    }

    if (readBytes < 0) {
        err("read", "could not read source file")
    }

    closeFile(src);
    closeFile(dst);

    free(newFileName);

    return 0;

}

void closeFile(int fd) {

    if (close(fd) < 0) {
        err("close", "could not close file")
    }

}

char *createNewFileName(char *oldName) {

    char *newName = (char *) malloc((strlen(oldName) + strlen("_kopija")) * sizeof(char));
    if (!newName) {
        err("malloc", "could not allocate memory")
    }
    strcpy(newName, oldName);
    strcat(newName, "_kopija");

    return newName;

}