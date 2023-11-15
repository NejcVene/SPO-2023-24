#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// ujamemo signal 10 (SIGUSR1)

// da pošlješ signal procesu uporabi ukaz: kill -s SIGUSER1 

void sigHandler(int sigNum) {

    printf("Prejeml sem signal %d\n", sigNum);

}

int main(void) {

    // dispozicija
    signal(SIGUSR1, sigHandler);

    while(1);

    return 0;

}