#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(char *, char *[]);
int parse2(char *, char *[]);

int main(void) {

    char *args[64];
    char str[] = "  gcc     -o      main        main.c      -Wall       -lm     -lpthead        -fopenmp";
    int el = parse2(str, args);

    for (int i = 0; i<el; i++) {
        printf("%s (%p)\n", args[i], &args[i]);
    }

    return 0;

}

int parse2(char command[], char *args[]) {

    int commandIndex = 0, argsIndex = 0, isEmpty = 0;
    while (command[commandIndex] != '\0') {
        if (command[commandIndex] == ' ' || command[commandIndex] == '\t') {
            command[commandIndex] = '\0';
            isEmpty = 0;
        } else {
            if (isEmpty == 0) {
                args[argsIndex++] = &command[commandIndex];
                isEmpty = 1;
            }
        }
        commandIndex++;
    }

    return argsIndex;

}

int parse(char command[], char *args[]) {

    int index = 0;
    args[index++] = command;
    for (int i = 0; i<strlen(command); i++) {
        if (command[i] == ' ') {
            printf("Found space at %d (%p)\n", i, &command[i + 1]);
            args[index++] = &command[i + 1];
            // command[i] = '\0';
        }
    }

    printf("Index = %d\n", index);
    if (index > 1) {
        for (int i = 0; i<index; i++) {
            // printf("%c\n", *(args[i] - 1));
            *(args[i] - 1) = '\0';
        }
    }
    args[index] = NULL;

    printf("DONE\n");
    return index;

}

/*

while(*p != '\n') {
    if (*p == ' ' || *p == '\t') {
        poln = 0;
        *p = '\0';
        p++;
    } else {
        if (poln == 0) {
            *args = p;
            p++;
        }
    }
}

*/