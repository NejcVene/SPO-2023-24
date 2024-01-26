#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    struct stat buffer;

    if (stat("p1.c", &buffer) < 0) {
        err("stat", "could not stat file")
    }

    if (S_ISREG(buffer.st_mode)) {
        printf("A regular file\n");
    }

    return 0;

}