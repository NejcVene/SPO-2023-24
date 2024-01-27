#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(void) {

    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, BUFFER_SIZE) == NULL) {
        perror("getcwd");
        exit(1);
    }

    printf("PWD: %s\n", buffer);

    return 0;

}