#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int status = system("ls -la | grep 'p1'");
    
    printf("Status %d\n", status);

    return 0;

}