#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define err(msg) { fprintf(stderr, "Napaka: %s\n", msg); exit(1); }

int main(int argc, char **args) {

    umask(0);
    if (open("file1", O_WRONLY | O_CREAT | O_TRUNC, 0666) < 0) {
        err("open file1")
    }
    // dobimo -> -rw-rw-rw- 1 nejcv nejcv     0 Oct 18 15:00 file1
    umask(0066);
    if (open("file2", O_WRONLY | O_CREAT | O_TRUNC, 0666) < 0) {
        err("open file2")
    }
    // dobimo -> -rw------- 1 nejcv nejcv     0 Oct 18 15:00 file2
    /*
    nismo uporabili close niti nismo shranili fd-jev, ker jih trenutno ne rabimo
    */

    return 0;

}