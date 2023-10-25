#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void printMode(mode_t);
void callChmod(char *, long int);

int main(int argc, char **argv) {

    if (argc < 3) {
        err("To few arguments.", "Usage: <mode> <filename>")
    }

    struct stat statBuf;
    char *remainder;
    long int mode = strtol(argv[1], &remainder, 8);
    if (mode == 0) {
        switch (argv[1][0]) {
            case 'u':
                printf("Going U\n");
                callChmod(argv[2], argv[1][1] == '+' ? (statBuf.st_mode) : (statBuf.st_mode & ~S_ISUID));
                break;
            case 'g':
                break;
            default:
                err("Unknown argument", argv[1])
                break;
        }
    }
    callChmod(argv[2], mode);
    if (stat(argv[2], &statBuf) < 0) {
        err("Could not stat file", argv[1])
    }
    printMode(statBuf.st_mode);
    

    return 0;

}

void printMode(mode_t st_mode) {

    char fileInfo[50];
    char *filePermissions[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    // add to string and determinde mode (permissions)
    sprintf(fileInfo, "%s %s%s%s",
            "Nova dovoljenja:",
            filePermissions[(st_mode >> 6) & 7],
            filePermissions[(st_mode >> 3) & 7],
            filePermissions[st_mode & 7]
           );
    // check for permission bits 
    if (S_ISUID & st_mode) {
        fileInfo[2] = 's';
    }
    if (S_ISGID & st_mode) {
        fileInfo[5] = 'S';
    }
    if (S_ISVTX & st_mode) {
        fileInfo[8] = 'T';
    }
    printf("%s\n", fileInfo);

}

void callChmod(char *file, long int mode) {

    printf("%lo\n", mode);
    if (chmod(file, mode) < 0) {
        err("Cannot change permissions for", file)
    }

}