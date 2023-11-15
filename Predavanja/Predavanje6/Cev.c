#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    int pfd[2]; // pipe file descriptor
    char buffer[512];

    if (pipe(pfd) < 0) {
        perror("Pipe ne dela");
    }
    write(pfd[1], "abcde\n", sizeof("abcde\n"));
    read(pfd[0], buffer, 512);
    puts(buffer);

    return 0;

}