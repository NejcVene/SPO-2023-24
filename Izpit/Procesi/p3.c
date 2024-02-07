#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(void) {

    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        err("pid", "cannot fork")
    } else if (pid == 0) {
        // child process
        char string[10];
        scanf("%s", string);

        if (execlp("stat", "stat", string, (char *) NULL) < 0) {
            err("execlp", "Cannot execlp")
        }

        exit(0);
    } else {
        // parent process
        wait(&status);
        printf("Child exited with status: %d\n", status);
        if (WIFEXITED(status)) {
            printf("Child exited normaly\n");
        } else {
            printf("Child exited abnormaly\n");
        }
    }



    return 0;

}