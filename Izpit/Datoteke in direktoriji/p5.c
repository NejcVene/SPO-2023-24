#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utime.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    struct utimbuf times;
    times.actime = 0; // this will set atime and mtime to 1 jan 1970
    times.modtime = 0;

    if (utime("new_dat.txt", NULL) < 0) {
        err("utime", "could not change time")
    }

    return 0;

}