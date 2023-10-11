#include <stdio.h>
#include <fcntl.h>

/*
errno() -> vzame zadnjo vrednost generirane napake, preveri kaj je to in izpiše.
*/

int main(void) {

    // odpremo datoteko file1 v načinu samo za branje
    int fd = open("file1", O_RDONLY); // fd kot file desc. (opiskik)
    if (fd < 0) {
        // printf("Ne morem odpreti datoteke\n");
        perror("Napaka.\n"); // boljše tole dat za napako
    }

    return 0;

}