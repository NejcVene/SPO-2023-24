#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define err(msg) { fprintf(stderr, "Napaka: %s\n", msg); exit(1); }

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Uporaba: %s <filename>\n", argv[0]);
        exit(1);
    }
    struct stat buf; // rabmo primerek te strukture

    // rabimo, da ta stuktura obstaja. Mi nato samo podamo kazalec do nje.
    if (stat(argv[1], &buf) < 0) {
        err("stat")
    }
    printf("%X\n", buf.st_mode);
    if (S_ISREG(buf.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(buf.st_mode)) {
        printf("Je direktorij\n");
    } else if (S_ISCHR(buf.st_mode)) {
        printf("Je znakovna naprava\n");
    } else if (S_ISBLK(buf.st_mode)) {
        printf("Bločna posebna datoteka\n");
    } else {
        printf("Unknown device\n");
    }
    // za tip.c dobimo 81A4

    return 0;

}