#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define err(msg, ime) { fprintf(stderr, "Error: %s %s\n", msg, ime); exit(1); }

int main(int argc, char **argv) {

    DIR *dp;
    struct dirent *dirP;
    if (argc != 2) {
        err("Command line", "to few arguments")
    }
    dp = opendir(argv[1]);
    if (!dp) { // preveri, če je NULL
        err("Ne morem odpreti direktorija", argv[1])
    }

    while ((dirP = readdir(dp)) != NULL) {
        printf("Ime: %s\n", dirP->d_name);
    }
    /* tole skoraj dela, ampak se naredi segmentation fault
    do {
        dirP = readdir(dp);
        printf("Ime: %s\n", dirP->d_name);
        // write(STDOUT_FILENO, dirP->d_name, strlen(dirP->d_name));
    } while (dirP != NULL); // lahko tudi while (dirP);
    */

    if (closedir(dp) < 0) {
        err("closedir na", argv[1]);
    }

    return 0;

}