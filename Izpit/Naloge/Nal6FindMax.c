#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    char buffer[BUFFER_SIZE];
    char targetFile[BUFFER_SIZE];
    off_t maxFileSize = 0;
    struct dirent *dirEntry;
    struct stat fileStat;
    DIR *d;

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        printf("Buffer: %s\n", buffer);
        if (!(d = opendir(buffer))) {
            err("opendir", "cannot open specified directory")
        }
        memset(buffer, 0, BUFFER_SIZE);
        while ((dirEntry = readdir(d)) != NULL) {
            if (stat(dirEntry->d_name, &fileStat) < 0) {
                err("stat", "cannot get file info")
            }
            if (S_ISREG(fileStat.st_mode)) {
                if (fileStat.st_size > maxFileSize) {
                    maxFileSize = fileStat.st_size;
                    strcpy(targetFile, dirEntry->d_name);
                }
            }
        }
        if (closedir(d) < 0) {
            err("closedir", "cannot close provided directory")
        }
        printf("%s (%ld)\n", targetFile, maxFileSize);
        maxFileSize = 0;
        memset(targetFile, 0, BUFFER_SIZE);
    }

    return 0;

}