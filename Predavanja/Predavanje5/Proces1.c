#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    // parent tega procesa je lupina (shell), zato je Parent PID vedno enak.

    printf("ID procesa je %d\nParent PID: %d\nUID: %d\n", 
            getpid(),
            getppid(),
            getuid());

    return 0;

}