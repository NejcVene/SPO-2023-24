#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> // za read in close

#define BUFF_SIZE 50

int main(void) {

    // odpremo datoteko file1 v načinu samo za branje
    int fd = open("file1", O_RDONLY); // fd kot file desc. (opiskik)
    int n; // število prebranih bajtov
    char buff[BUFF_SIZE]; // kam grejo prebrani podatki
    if (fd < 0) {
        // printf("Ne morem odpreti datoteke\n");
        perror("Napaka.\n"); // boljše tole dat za napako
        exit(1); // izhod s statusam 1
    }
    printf("File desc. (source): %d\n", fd);
    while ((n = read(fd, buff, BUFF_SIZE)) < 0) {
        
    }
    
    /*
    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);
    
    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);

    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);
    */
    if (close(fd) < 0) {
        perror("Nemorem zapredi datoteke.\n");
    } // zapri datoteko

    return 0;

}