#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 128
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

char *newFileName(char *);

int main(int argc, char **args) {

    // check that there are two arguments present
    if (argc < 2) {
        err("To few arguments")
    }
    int readFd, writeFd, readBytes;
    // try to open the file for reading
    if ((readFd = open(args[1], O_RDONLY)) < 0) {
        err("Cant open file to read")
    }
    // create the new file name
    char *createdFileName = newFileName(args[1]);
    // try to create/truncate the file to write to
    if ((writeFd = open(createdFileName, O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0) {
        err("Cant write to destination file")
    }
    // free the allocated memory for new file name, as it is not needed anymore
    free(createdFileName);
    char buffer[READ_SIZE];
    // try and read 128 bytes at a time
    while ((readBytes = read(readFd, buffer, READ_SIZE)) > 0) {
        // try and write the read bytes
        if (write(writeFd, buffer, readBytes) != readBytes) {
            err("Write failed")
        }
    }
    // try to close both files
    if (close(readFd) < 0 || close(writeFd) < 0) {
        err("Cannot close files")
    }

    return 0;

}

char *newFileName(char *readFileName) {

    // try to allocate memory to create a new file name
    char *createdFileName = (char *) malloc(sizeof(char) * (strlen(readFileName) + 8)); // len("_kopija") = 7 + '\0' = 8
    if (!createdFileName) {
        err("Could not allocate memory")
    }
    // copy old file name
    strcpy(createdFileName, readFileName);
    // append "_kopija" to old file name
    strcat(createdFileName, "_kopija");

    return createdFileName;

}