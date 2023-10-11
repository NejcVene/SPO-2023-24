#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

/*
Tukaj za kreiranje 2D tabele uporabimo kazalec na kazalec
*/

int main(void) {

    int **p = (int **) malloc(sizeof(int *) * SIZE);
    if (!p) {
        perror("Could not allocate inital memory");
        return -1;
    }
    for (int i = 0; i<SIZE; i++) {
        p[i] = (int *) malloc(sizeof(int) * SIZE);
        if (!p[i]) {
            perror("Could not allocate memory segment");
            return -2;
        }
    }

    for (int i = 0; i<SIZE; i++) {
        for (int j = 0; j<SIZE; j++) {
            p[i][j] = i;
            printf("%d ", p[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i<SIZE; i++) {
        free(p[i]);
    }
    free(p);

    return 0;

}