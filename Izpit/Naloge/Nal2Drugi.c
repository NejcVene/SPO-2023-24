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

int countWords(char *);

int main(void) {

    key_t key;
    int queueID;
    struct mymesg msg;

    if ((key = ftok("/home", 'a')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((queueID = msgget(key, 0644 | IPC_CREAT)) < 0) {
        err("msgget", "cannot open/create queue")
    }

    if (msgrcv(queueID, &msg, sizeof(msg.mtext), msg.mtype, 0) < 0) {
        err("msgrcv", "cannot receive data")
    }

    printf("Word count: %d\n", countWords(msg.mtext));

    if (msgctl(queueID, IPC_RMID, NULL) < 0) {
        err("msgctl", "cannot delete queue")
    }

}

int countWords(char *buffer) {

    int count = 0, i = 0, isWord = 0;
    while (buffer[i] != '\0') {
        switch (buffer[i]) {
            case ' ':
            case '\t':
            case '\n':
            case '\0':
            case '\r':
                isWord = 0;
                break;
            default:
                if (isWord == 0) {
                    count++;
                    isWord = 1;
                }
                break;
        }
        i++;
    }

    return count;

}