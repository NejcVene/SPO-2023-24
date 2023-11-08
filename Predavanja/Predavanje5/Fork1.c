#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

int main(void) {

    pid_t pid;
    if ((pid = fork()) < 0) {
        err("Fork did not work")
    }
    if (pid == 0) { // otrok
        printf("Otrok: %d\nStarš: %d\n", getpid(), getppid());
    } else { // starš
        sleep(1);
        printf("Starš: %d\nMoj starš: %d\n", getpid(), getppid());
    }
    printf("Jaz sem %d, moj starš je %d\n", getpid(), getppid());

    return 0;

}