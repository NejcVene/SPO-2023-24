#include <stdio.h>
#include <stdlib.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

FILE *openFile(char *, char *);
void closeFile(FILE *);
FILE *findLine(FILE *, int);
void skipLine(FILE *);
void writeRemaining(FILE *, FILE *);
void appendLine(FILE *);
void deleteLine(FILE *, FILE *);
void insertLine(FILE *, FILE *);
void changeLine(FILE *, FILE *);
void renameRemove(char *);

int main(int argc, char **argv) {

    // check for arguments
    if (argc < 3) {
        err("To few arguments.", "Usage: <filename> a, d, i, c")
    }

    FILE *f;
    // which flag is selected
    switch (argv[2][0]) {
        case 'a': // append
            f = openFile(argv[1], "a");
            appendLine(f);
            closeFile(f);
            break;
        case 'd': // delete
            f = openFile(argv[1], "r+");
            deleteLine(f, findLine(f, atoi(argv[3])));
            closeFile(f);
            renameRemove(argv[1]);
            break;
        case 'i': // insert
            f = openFile(argv[1], "r");
            insertLine(f, findLine(f, atoi(argv[3])));
            closeFile(f);
            renameRemove(argv[1]);
            break;
        case 'c': // change
            f = openFile(argv[1], "r");
            changeLine(f, findLine(f, atoi(argv[3])));
            closeFile(f);
            renameRemove(argv[1]);
            break;
        default:
            err("Unknown flag", argv[2])
    }

    return 0;

}

FILE *openFile(char *fileName, char *mode) {

    // open file and return its pointer
    FILE *f = fopen(fileName, mode);
    if (!f) {
        err("Could not open file", fileName)
    }
    
    return f;

}

void closeFile(FILE *f) {

    // try to close file pointed to by f
    if (fclose(f) == EOF) {
        err("Could not close file", "closeFile")
    }

}

FILE *findLine(FILE *f, int n) {

    // read file till enough '\n' have been read
    int c, bytesRead = 0;
    while (n != 1 && (c = fgetc(f)) != EOF) {
        if (c == '\n') { // \n = LF
            n--;
        }
    }
    // tell where the file pointer is currently located
    if ((bytesRead = ftell(f)) == -1) {
        err("Could not find correct line", "ftell")
    }
    // that number will tell us how many unchanged bytes we have to copy into the new file
    char copyBuffer[bytesRead];
    // rewind to beginning and copy those bytes into buffer
    rewind(f);
    if (fread(copyBuffer, sizeof(copyBuffer), 1, f) != 1) {
        err("Could not find correct line", "fread")
    }
    // create a new file and write copied bytes into it
    FILE *tmpF = openFile("tmp", "w");

    if (fwrite(copyBuffer, sizeof(copyBuffer), 1, tmpF) != 1) {
        err("Could not find correct line", "fwrite")
    }
    
    // return tmp files' pointer
    return tmpF;

}

void skipLine(FILE *f) {

    // skip one line
    int c = 0;
    while (c != '\n' && (c = fgetc(f)) != EOF);

}

void writeRemaining(FILE *f, FILE *tmpF) {

    // write everything else into tmp
    char buffer[512];
    while ((fgets(buffer, 512, f)) != NULL) {
        if (fputs(buffer, tmpF) == EOF) {
            err("Could not write to file", "fputs")
        }
    }
    closeFile(tmpF);

}

void renameRemove(char *fileName) {

    // remove old file, and rename tmp to it
    if (remove(fileName) == -1) {
        err("Could not switch files", "remove")
    }
    if (rename("tmp", fileName) == -1) {
        err("Could not switch files", "rename")
    }

}

void appendLine(FILE *f) {

    // append line from stdin to file
    int c;
    while ((c = getchar()) != EOF) {
        if (fputc(c, f) == EOF) {
            err("Could not append to file", "fputc")
        }
    }

}

void deleteLine(FILE *f, FILE *tmpF) {

    // delete line specified by user
    skipLine(f);
    writeRemaining(f, tmpF);

}

void insertLine(FILE *f, FILE *tmpF) {

    // insert a new line specified by user
    appendLine(tmpF);
    writeRemaining(f, tmpF);

}

void changeLine(FILE *f, FILE *tmpF) {

    // change the line that was specified by user
    appendLine(tmpF);
    skipLine(f);
    writeRemaining(f, tmpF);

}