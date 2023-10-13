#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 50
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

int main(int argc, char **args) {

    int writeFd, readBytes;
    char buffer[BUFFER_SIZE];
    switch (argc) {
        case 2: // truncate
            if ((writeFd = open(args[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
                err("Could not open file")
            }
            break;
        case 3: // append
            if ((writeFd = open(args[2], O_WRONLY | O_CREAT | ((strcmp(args[1], "-a") == 0) ? (O_APPEND) : (O_TRUNC)), 0644)) < 0) {
                err("Could not open file")
            }
            break;
        default:
            err("To few or many arguments")
            break;
    }
    while ((readBytes = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if (write(writeFd, buffer, readBytes) != readBytes || write(STDOUT_FILENO, buffer, readBytes) != readBytes) {
            err("Could not write or print")
        }
    }

    return 0;

}

/*
    if ((writeFd = open(args[2], O_WRONLY | O_CREAT | ((strcmp(args[1], "-a") == 0) ? (O_APPEND) : (O_TRUNC)), 0644)) < 0) {
        err("Could not open file")
    }
*/