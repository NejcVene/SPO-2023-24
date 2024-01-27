#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    // opened stdout using it's fd with fdopen which returns a FILE *
    // basicaly, I've made a horriblle verion of printf
    FILE *f = fdopen(1, "w");
    // putc('a', f); // a macro, apperently
    // fputc('a', f);

    fputc('a', f);

    fclose(f);

    return 0;

}