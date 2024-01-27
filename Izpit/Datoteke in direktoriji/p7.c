#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    DIR *dir;
    struct dirent *sDir;

    if (!(dir = opendir("."))) {
        err("opendir", "could not open directory")
    }

    while ((sDir = readdir(dir)) != NULL) {
        printf("Name: %s (%ld)\n", sDir->d_name, sDir->d_ino);
    }

    if (closedir(dir) < 0) {
        err("closedir", "could not close directory")
    }

    return 0;

}