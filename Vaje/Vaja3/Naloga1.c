#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void determineFileType(mode_t, char *);

int main(int argc, char **argv) {

    DIR *dp;
    struct dirent *dirP;
    struct stat statBuf;
    char fileInfo[10];
    char *fileInfoP = fileInfo;

    // check number of arguments
    if (argc < 2) {
        err("To few arguments", "Usage: <name>")
    }

    // try to open the specified directory
    if (!(dp = opendir(argv[1]))) {
        err("Could not open target directory", argv[1])
    }

    // try to read directory contents
    while ((dirP = readdir(dp))) {
        // try to stat file
        if (stat(dirP->d_name, &statBuf) < 0) {
            err("Could not stat file", dirP->d_name) // fails becouse its not the full path
        }
        // determine its type and mode
        determineFileType(statBuf.st_mode, fileInfoP);
        // print result
        printf("%s %ld %s\n", fileInfo, statBuf.st_nlink, dirP->d_name);
    }

    // try to close directory
    if (closedir(dp) < 0) {
        err("Could not close target directory", argv[1])
    }

    return 0;

}

void determineFileType(mode_t st_mode, char *fileInfo) {

    char type;
    char *filePermissions[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    /*
    Indexes really matter in this array, as they are mapped out for specific permissions:
        0 = 0000
        1 = 0100
        2 = 0200
        3 = 0300
        4 = 0400
        5 = 0500
        6 = 0600
        7 = 0700
    */
    // determinte file type
    if (S_ISREG(st_mode)) {
        type = '-'; // regular file
    } else if (S_ISDIR(st_mode)) {
        type = 'd'; // directory file
    } else if (S_ISCHR(st_mode)) {
        type = 'c'; // character files
    } else if (S_ISBLK(st_mode)) {
        type = 'b'; // block files
    } else if (S_ISFIFO(st_mode)) {
        type = 'p'; // pipe files
    } else if (S_ISLNK(st_mode)) {
        type = 'l'; // link files
    } else if (S_ISSOCK(st_mode)) {
        type = 's'; // socket files
    } else {
        type = '/'; // unknown file
    }
    // add to string and determinde mode (permissions)
    sprintf(fileInfo, "%c%s%s%s", 
           type,
           filePermissions[(st_mode >> 6) & 7],
           filePermissions[(st_mode >> 3) & 7],
           filePermissions[st_mode & 7]
           );
    // check for permission bits 
    if (S_ISUID & st_mode) {
        fileInfo[3] = 's';
    }
    if (S_ISGID & st_mode) {
        fileInfo[6] = 'S';
    }
    if (S_ISVTX & st_mode) {
        fileInfo[9] = 'T';
    }

}