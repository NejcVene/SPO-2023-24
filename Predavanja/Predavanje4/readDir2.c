// program, ki vse datoteka v direktoriju
// ki so starejše od 100 sekund doda k imenu
// "_1", torej "imeDatoteke_1"

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

    if (closedir(dp) < 0) {
        err("closedir na", argv[1]);
    }

    return 0;

}