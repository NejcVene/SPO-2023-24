#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void copyFile(char *);
void closeFile(FILE *);

int main(void) {

    int readBytes;
    char buffer[BUFFER_SIZE];

    while ((readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        copyFile(buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }

    if (readBytes < 0) {
        err("read", "cannot read from STDIN")
    }

    return 0;

}

void copyFile(char *path) {

    FILE *f, *f2;
    char content[BUFFER_SIZE];
    struct stat statBuf;

    path[strlen(path) - 1] = '\0';
    if (!(f = fopen(path, "r"))) {
        // assume that file does not exist
        if (!(f = fopen(path, "w"))) {
            err("fopen", "cannot create a file")
        }
        closeFile(f);
        return;
    }

    strcat(path, ".bkp");
    if (!(f2 = fopen(path, "w"))) {
        err("fopen", "cannot create backup file")
    }

    while (fgets(content, BUFFER_SIZE - 1, f) != NULL) {
        if (fputs(content, f2) == EOF) {
            err("fputs", "cannot copy from source file")
        }
    }

    if (stat(path, &statBuf) < 0) {
        err("stat", "cannot stat file")
    }

    if (chmod(path, statBuf.st_mode & ~(S_IWUSR | S_IWGRP | S_IWOTH)) < 0) {
        err("chmod", "cannot change backup file permissions")
    }

    closeFile(f);
    closeFile(f2);

}

void closeFile(FILE *f) {

    if (fclose(f) == EOF) {
        err("close", "cannot close file")
    }

}