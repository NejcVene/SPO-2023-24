#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

    if (pause() == -1) {
        perror("Error");
        exit(1);
    }

    return 0;

}