#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

#define BUFFER_SET_POSITION_SIZE 1
#define BUFFER_SIZE              256

int readFile(int, char *, int, int*);
void writeFile(int, char *, int);
void moveCursorPosition(int, char *);

int main(int argc, char **argv) {

    if (argc < 4 || strcmp(argv[1], "-n") != 0) {
        err("Wrong arguments")
    }
    int readFd, writeFd, readBytes, delLineCount = atoi(argv[2]), lineCounter = 1, newOffset, c = 0, sumOfBytes = 0;
    char buffer[BUFFER_SIZE], bufferSetPos[BUFFER_SET_POSITION_SIZE];
    // try to open the file that was specified and open/create a tmp file to write new content to
    if ((readFd = open(argv[3], O_RDONLY)) < 0 || (writeFd = open("tmp", O_WRONLY | O_TRUNC | O_CREAT, 0644)) < 0) {
        err("Could not open source file")
    }
    // repeat until c is 0
    // this is used to write content BEFORE the line to be deleted
    while ((c = readFile(readFd, buffer, delLineCount, &lineCounter))) {
        sumOfBytes += c;
        writeFile(writeFd, buffer, c);
    }
    // seek to beginning of file
    if ((newOffset = lseek(readFd, 0, SEEK_CUR)) == -1) {
        err("Could not seek to beginning of file")
    }
    // by using sumOfBytes seek to the last line before the line to be deleted
    if ((newOffset = lseek(readFd, sumOfBytes, SEEK_SET)) == -1) {
        err("Could not seek to new posititon")
    }
    // skip the line to be deleted
    moveCursorPosition(readFd, bufferSetPos);
    // write everything after the skiped line
    while ((readBytes = read(readFd, buffer, BUFFER_SIZE)) > 0) {
        writeFile(writeFd, buffer, readBytes);
    }
    // try and get rid of all links to the file that user specified.
    // removing them means that the file will be deleted.
    if (unlink(argv[3]) == -1) {
        err("Could not unlink")
    }
    // try to rename "tmp" to whatever file user specified
    if (rename("tmp", argv[3]) == -1) {
        err("Could not rename")
    }
    // try and close both files
    if (close(readFd) < 0 || close(writeFd) < 0) {
        err("Could not close source file")
    }

    return 0;

}

int readFile(int fd, char *buffer, int delLineCount, int *pN) {

    if (*pN == delLineCount) {
        return 0;
    }
    // byteCounter is used, to track the number of bytes it takes
    // to reach the correct line
    int readBytes, byteCounter = 0;
    while ((readBytes = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // go throug read bytes one at a time
        for (int i = 0; i<readBytes; i++) {
            byteCounter++;
            // check, if char is a new line
            if (buffer[i] == '\n') {
                // increment line number counter
                (*pN)++;
                // line to be deleted is found
                if (*pN == delLineCount) {
                    // return how many bytes it takes to get to that line
                    return byteCounter;
                }
            }
        }
        // if this is true, then it means we read 256 bytes without finding
        // the correct line. This then means its not in this buffer or is
        // partly in it
        if (readBytes == BUFFER_SIZE) {
            return byteCounter;
        }
    }

    return 0;

}

// write bytesToWrite bytes from buffer into a file specified by fd 
void writeFile(int fd, char *buffer, int bytesToWrite) {

    if (write(fd, buffer, bytesToWrite) != bytesToWrite) {
        err("Could now write in function")
    }

}

// used to skip one whole line
void moveCursorPosition(int fd, char *buffer) {

    int readBytes;
    // keep reading one byte till we reach \n.
    while (buffer[0] != '\n') {
        if ((readBytes = read(fd, buffer, BUFFER_SET_POSITION_SIZE)) == -1) {
            err("Cound not read")
        }
    }

}
