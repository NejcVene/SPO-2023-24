#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    int pfd[2]; // pipe file descriptor
    char buffer[512];
    pid_t pid;
    int n;

    if (pipe(pfd) < 0) {
        perror("Pipe ne dela");
    }

    if ((pid = fork()) < 0) {
        perror("Fork ne dela");
    } else if (pid == 0) { // otrok bo bral
        close(pfd[1]);
        while ((n = read(pfd[0], buffer, 512)) > 0) {
            if (write(1, buffer, n) != n) {
                perror("Write na stdout ne dela");
            }
        }
        close(pfd[0]);
    } else { // starš bo pisal
        close(pfd[0]);
        write(pfd[1], "abcde", sizeof("abcde"));
        write(pfd[1], "fghij", sizeof("abcde"));
        close(pfd[1]);

        wait(NULL); // ker smo dal NULL to pomeni, da nas ne zanima kakšen je izhod
    }

    return 0;

}