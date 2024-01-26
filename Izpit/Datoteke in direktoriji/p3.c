#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    if (truncate("test_dat.txt", 10) < 0) {
        err("truncate", "could not truncate file")
    }

    return 0;

}