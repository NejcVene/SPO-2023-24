#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define err(msg) { perror(msg); exit(1); }

int main(void) {

    FILE *f = fopen("datoteka_ki_ne_obstaja.txt", "r");

    perror("Datoteka ne obstaja");

    char *err = strerror(ENOENT);
    printf("strerror: %s\n", err);

    return 0;

}
