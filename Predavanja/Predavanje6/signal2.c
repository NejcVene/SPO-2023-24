#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigHandler(int sigNum) {

    printf("Prejel sem signal %d\n", sigNum);

}

int main(void) {

    // dispozicija
    signal(SIGUSR1, sigHandler);
    signal(SIGALRM, sigHandler);

    alarm(30);
    pause();
    printf("Ostalo je še %d sekund do konca\n", alarm(0)); // ukine VSE alarme in pove, koliko je do konca

    return 0;

}