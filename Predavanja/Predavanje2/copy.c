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

    int fd2 = open("file1_COPY", O_WRONLY | O_CREAT | O_TRUNC, 0644); // če še ne obstaja ga kreiraj, če pa že ga pa nadomesti
    // 0664 so pravice za to datoteko (u = rw, g = r, o = r)
    // 0 pred to cifro gre za to, da se ve da gre za osmiški zapis.
    if (fd2 < 0) {
        perror("Napaja fd2\n");
        exit(2);
    }
    printf("File desc. (source): %d\nFile desc. (dest): %d\n", fd, fd2);
    /*
    while ((n = read(fd, buff, BUFF_SIZE)) > 0) {
        // printf("Število prebranih bajtov: %d\n", n);
        if (write(fd2, buff, n) != n) {
            perror("Write ne prebere n bajtov");
            exit(3);
        }
    }
    */
    do {
        n = read(fd, buff, BUFF_SIZE);
        if (n < 0) {
            perror("Napaka read");
            exit(5);
        }
        if (write(fd2, buff, n) != n) {
            perror("Write ne prebere n bajtov");
            exit(3);
        }
    } while (n > 0);
    
    /*
    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);
    
    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);

    n = read(fd, buff, BUFF_SIZE); // preberi datoteko
    printf("Število prebranih bajtov: %d\n", n);
    */
    if (close(fd) < 0 || close(fd2) < 0) {
        perror("Nemorem zapredi datoteke.\n");
    } // zapri datoteko

    return 0;

}