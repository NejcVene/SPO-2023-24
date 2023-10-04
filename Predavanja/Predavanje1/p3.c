#include <stdio.h>

/*
v[2] = *(p + 2) je isto

v[i][j] = *(*(v + i) + j) je isto

v[0][0] = *v = **v je isto
*/
/*
Alokacija:
int **p = (int **) malloc(N * sizeof(int *))
if (p == NULL) {
    napaka
}
for (int i = 0; i<N; i++) {
    p[i] = (int *) malloc(N * sizeof(int));
    if (p[i] == NULL) {
        napaka
    }
}
dealokacija:
for (int i = 0; i<N; i++) {
    free(p[i]);
}
if (p) { pogleda, če obstaja (je različen od NULL)
    free(p)
} 
*/
int main(void) {

    int v[2][3] = {{10, 20, 30}, {50, 60, 70}};

    printf("v: %p, v[0]: %p, v[0][0]: %d\n", v, v[0], v[0][0]);

    return 0;

}