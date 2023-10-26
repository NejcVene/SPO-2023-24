#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void printMode(mode_t);
void callChmod(char *, mode_t);
void getFileStat(char *, struct stat*);

int main(int argc, char **argv) {

    // check for arguments
    if (argc < 3) {
        err("To few arguments.", "Usage: <mode> <filename>")
    }

    struct stat statBuf;
    char *remainder;
    mode_t mode = strtol(argv[1], &remainder, 8);
    // if mode is 0 then strtol() failed to convert to base 8, meaning (probably) that argv[1] is not a number
    if (mode == 0) {
        // get file stat
        getFileStat(argv[2], &statBuf);
        // check for correct argument
        if (strcmp(argv[1], "u+s") == 0) {
            callChmod(argv[2], statBuf.st_mode | S_ISUID);
        } else if (strcmp(argv[1], "u-s") == 0) {
            callChmod(argv[2], statBuf.st_mode & ~S_ISUID);
        } else if (strcmp(argv[1], "g+s") == 0) {
            callChmod(argv[2], statBuf.st_mode | S_ISGID);
        } else if (strcmp(argv[1], "g-s") == 0) {
            callChmod(argv[2], statBuf.st_mode & ~S_ISGID);
        } else {
            err("Unknown argument", argv[1])
        }
        // get new file stat and print it out
        getFileStat(argv[2], &statBuf);
        printMode(statBuf.st_mode);
        return 0;
    }
    // if we are here, then strtol() converted a number to base 8
    // apply mode
    callChmod(argv[2], mode);
    // get file stat
    getFileStat(argv[2], &statBuf);
    // print new file mode
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
        fileInfo[19] = 's';
    }
    if (S_ISGID & st_mode) {
        fileInfo[22] = 'S';
    }
    if (S_ISVTX & st_mode) {
        fileInfo[25] = 'T';
    }
    printf("%s\n", fileInfo);

}

void callChmod(char *file, mode_t mode) {

    // try to run chmod on file
    if (chmod(file, mode) < 0) {
        err("Cannot change permissions for", file)
    }

}

void getFileStat(char *file, struct stat *statBuf) {

    // try to get stat for file
    if (stat(file, statBuf) < 0) {
        err("Could not stat file", file)
    }

}

/*
switch (argv[1][0]) {
            case 'u':
                callChmod(argv[2], (argv[1][1] == '+' && argv[1][2] == 's') ? (statBuf.st_mode | S_ISUID) : (statBuf.st_mode & ~S_ISUID));
                break;
            case 'g':
                callChmod(argv[2], (argv[1][1] == '+' && argv[1][2] == 's') ? (statBuf.st_mode | S_ISGID) : (statBuf.st_mode & ~S_ISGID));
                break;
            default:
                err("Unknown argument", argv[1])
                break;
        }
*/