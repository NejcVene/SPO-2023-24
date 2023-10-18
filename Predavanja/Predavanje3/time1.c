#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <fcntl.h>

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
    int fd;
    struct utimbuf timebuf;
    if ((fd = open(argv[1], O_TRUNC, O_RDWR)) < 0) {
        err("open")
    }
    // program je te dve datoteki odpiral, vendar se časa dostopa nista spremenila (kot, da se ne bi uporabila oz. odprla)
    timebuf.actime = buf.st_atime; // čas dostopa
    timebuf.modtime = buf.st_mtime; // čas modifikacije
    if (utime(argv[1], &timebuf) < 0) {
        err("utime")
    }

    return 0;

}