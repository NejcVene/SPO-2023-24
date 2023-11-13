#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(char *, char *[]);

int main(void) {

    char *args[64];
    char str[] = "gcc -o main main.c -Wall -lm -lpthead -fopenmp";
    int el = parse(str, args);

    for (int i = 0; i<el; i++) {
        printf("%s (%p)\n", args[i], &args[i]);
    }

    return 0;

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