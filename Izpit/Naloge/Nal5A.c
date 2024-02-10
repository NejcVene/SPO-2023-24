#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>
#include <time.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }
#define BUFFER_SIZE 1024
#define BIG_BUFFER 8 * 1000

void childProcess(int, int, int, char *, int);

struct mymesg {
    long mtype;
    char mtext[BUFFER_SIZE];
};

int main(int argc, char **argv) {

    if (argc < 3) {
        err("To few arguments.", "Usage: <file> <num. of proc.>")
    }

    char *filename = argv[1];
    char buffer[BIG_BUFFER];
    int n = atoi(argv[2]);
    key_t key;
    int queueID, fd, readBytes, status;
    pid_t pid, exited;

    if ((key = ftok("/home", 'b')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((queueID = msgget(key, 0644 |IPC_CREAT)) < 0) {
        err("msgget", "cannot open/create queue")
    }

    if ((fd = open(filename, O_RDONLY)) < 0) {
        err("open", "cannot open file")
    }

    if ((readBytes = read(fd, buffer, BIG_BUFFER)) < 0) {
        err("read", "cannot read file")
    }

    if (close(fd) < 0) {
        err("close", "cannot close file")
    }

    for (int i = 0; i<n; i++) {
        if ((pid = fork()) < 0) {
            err("fork", "cannot create process")
        } else if (pid == 0) {
            // child process

            childProcess(i, n, queueID, buffer, strlen(buffer));
            exit(0);

        }
    }

    for (int i = 0; i<n; i++) {
        exited = waitpid(-1, &status, WNOHANG);
        // printf("Child %d exited with %d\n", exited, status);
    }

    return 0;

}

void childProcess(int childID, int n, int queueID, char *buffer, int bufferSize) {

    int start = ((bufferSize - 1) / n) * childID,
        end = ((bufferSize - 1) / n) * (childID + 1),
        rnd;
    struct mymesg msg;
    msg.mtype = childID + 1;

    if (childID == n - 1) {
        end = bufferSize;
    }

    // printf("Start: %d End: %d %d %d\n", start, end, getpid(), end - start);
    strncpy(msg.mtext, buffer + start, end - start);
    msg.mtext[strlen(msg.mtext)] = '\0'; 
    // strcpy(msg.mtext, buff);
    printf("%s [%ld]\n\n", msg.mtext, msg.mtype);

    srand(time(NULL));
    rnd = rand() % 10;
    printf("Going to sleep for: %d\n", rnd);

    sleep(rnd);

    if (msgsnd(queueID, &msg, sizeof(msg.mtext), 0) < 0) {
        err("msgsnd", "cannot send message")
    }

}