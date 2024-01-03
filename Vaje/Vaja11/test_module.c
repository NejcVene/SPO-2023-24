#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }
#define BUFFER_SIZE 100

void readFromModule(FILE *);
void writeToModule(FILE *);
void closeFile(FILE *);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <-w> <-r>")
    }

    FILE *module;
    if (!(module = fopen("/dev/mem0", (strcmp(argv[1], "-r") == 0) ? "r" : "w"))) {
        err("fopen", "could not open module mem0")
    }

    if (strcmp(argv[1], "-r") == 0) {
        readFromModule(module);
    } else if (strcmp(argv[1], "-w") == 0) {
        writeToModule(module);
    } else {
        err("Could not read or write to mem0", "unknown arguments")
    }

    return 0;

}

void readFromModule(FILE *module) {

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, module)) {
        printf("%s", buffer);
    }
    closeFile(module);

}

void writeToModule(FILE *module) {

    char buffer[BUFFER_SIZE];
    printf("CTRL + D to exit\n");
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (fwrite(buffer, 1, strlen(buffer), module) != strlen(buffer)) {
            err("fwrite", "could not write to module")
        }
    }
    closeFile(module);

}

void closeFile(FILE *f) {

    if (fclose(f) == EOF) {
        err("fclose", "could not close file")
    }

}
