#include <stdio.h>
#include <stdlib.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    if (argc < 3) {
        err("To few arguments", "Usage <old name> <new name>")
    }

    if (rename(argv[1], argv[2]) < 0) {
        err("rename", "could not rename file")
    }

    return 0;

}