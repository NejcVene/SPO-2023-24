#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }
#define BUFFER_SIZE 1024

struct mymesg {
    long mtype;
    char mtext[BUFFER_SIZE];
};

int main(int argc, char **argv) {

    if (argc < 3) {
        err("To few arguments.", "Usage: <file> <num. of proc.>")
    }

    char *filename = argv[1];
    int n = atoi(argv[2]);
    key_t key;
    int queueID, fd, status;
    pid_t pid;
    struct mymesg msg;
    char *newFileName;

    if ((key = ftok("/home", 'b')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((queueID = msgget(key, 0644 |IPC_CREAT)) < 0) {
        err("msgget", "cannot open/create queue")
    }

    if ((fd = open("kopija", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        err("open", "cannot open/create/truncate file")
    }

    for (int i = 0; i<n; i++) {
        if (msgrcv(queueID, &msg, sizeof(msg.mtext), i + 1, 0) < 0) {
            err("msgrcv", "cannot receive message")
        }
        if (write(fd, msg.mtext, strlen(msg.mtext)) != strlen(msg.mtext)) {
            err("write", "cannot write to file")
        }
    }

    if (!(newFileName = (char *) malloc((strlen(filename) + 1) * sizeof(char)))) {
        err("malloc", "cannot allocate memory")
    }
    strcpy(newFileName, filename);
    newFileName[strlen(newFileName)] = '\0';

    if ((pid = vfork()) < 0) {
        err("vfork", "cannot create process")
    } else if (pid == 0) {
        // child process

        if (execlp("diff", "diff", "-s", newFileName, "kopija", (char *) 0) < 0) {
            err("execlp", "cannot run program diff")
        }

        exit(0);
    } else {
        // parent process
        pid = wait(&status);
        printf("Child %d exited with %d\n", pid, status);
    }

    if (msgctl(queueID, IPC_RMID, 0) < 0) {
        err("msgctl", "cannot remove queue")
    }

    free(newFileName);

    return 0;

}