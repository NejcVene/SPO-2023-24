#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> // za read in close

#define err(msg) { fprintf(stderr, "Napaka: %s\n", msg); exit(1); }

int main(void) {

    int fd = open("file22", O_RDONLY);
    if (fd < 0) {
        err("Datoteka ne obstaja")
    }

}