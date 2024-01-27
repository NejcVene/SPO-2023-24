#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareString(char *, char *);

int main(void) {

    printf("Same string? %d\n", compareString("s1", "s2"));
    printf("Same string? %d\n", compareString("s1", "s1"));
    printf("Same string? %d\n", compareString("samokolnica", "avto"));
    printf("Same string? %d\n", compareString("samokolnica", "samokolnica"));



    return 0;

}

int compareString(char *s1, char *s2) {

    if (strlen(s1) != strlen(s2)) {
        return 0;
    }

    for (size_t i = 0; i<strlen(s1); i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }

    return 1;

}