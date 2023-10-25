#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 1


int main(void) {

    int n; // število prebranih bajtov
    char buff[BUFF_SIZE]; // kam grejo prebrani podatki

    while ((n = read(STDIN_FILENO, buff, BUFF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buff, n) != n) { // ne pomeni, da je vedno napaka, zato imamo rajši navaden printf
            printf("Napaka\n");
            exit(1);
        }
    }
    if (n < 0) {
        perror("Napaka pri branju\n");
    }

    return 0;

}