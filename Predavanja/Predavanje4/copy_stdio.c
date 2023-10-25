#include <stdio.h>
#include <stdlib.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

int main(void) {

    // tole je vrstično medpomnenje
    int c;
    while ((c = getchar()) != EOF) { // beri iz stdin 1 bajt
        if (putchar(c) == EOF) { // izpiše na stdout (c izpiše na stdout)
            err("putchar ") // če vrne EOF je napaka (kar naj bi bilo zelo redko, da ne more izpisati 1 bajte)
        }
    }

    if (ferror(stdin)) {
        err("getchar")
    }

    return 0;

}