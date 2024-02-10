#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_FILE_SIZE 8 * 1000
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

struct mymesg {
    long mtype;
    char mtext[MAX_FILE_SIZE];
};

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <filename>")
    }

    char buffer[MAX_FILE_SIZE];
    int fileFd, readBytes;
    key_t key;
    struct mymesg msg;
    int queueID;
    msg.mtype = 1;
    
    if ((key = ftok("/home", 'a')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((queueID = msgget(key, 0644 | IPC_CREAT)) < 0) {
        err("msgget", "cannot create/get queue")
    }

    if ((fileFd = open(argv[1], O_RDONLY)) < 0) {
        err("open", "cannot read file")
    }

    if ((readBytes = read(fileFd, buffer, MAX_FILE_SIZE)) < 0) {
        err("read", "cannot read from file")
    }

    strcpy(msg.mtext, buffer);
    
    if (msgsnd(queueID, &msg, strlen(msg.mtext), 0) < 0) {
        err("msgsnd", "cannot send string")
    }

    if (close(fileFd) < 0) {
        err("close", "cannot close file")
    }

    return 0;

}