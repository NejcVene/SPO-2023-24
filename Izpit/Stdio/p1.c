#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }


int main(void) {

    char buffer[BUFFER_SIZE];

    if (setvbuf(stdout, buffer, _IOFBF, BUFFER_SIZE)) {
        err("setvbuf", "cant change to full buffering")
    }
    printf("Hello");
    fflush(stdout);

    if (setvbuf(stdout, buffer, _IOLBF, BUFFER_SIZE)) {
        err("setvbuf", "cant change to line buffering")
    }
    printf(", wor");
    // commenting stuff out below this line will only print "Hello" to stdout
    putchar('\n');

    if (setvbuf(stdout, buffer, _IONBF, BUFFER_SIZE)) {
        err("setvbuf", "cant change to no buffering")
    }

    printf("%s", "ld!");

    return 0;

}