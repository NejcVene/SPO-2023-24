#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_FILE_SIZE 16 * 1000
#define err(msg, reason) { fprintf(stderr, "Erorr: %s %s\n", msg, reason); exit(1); }

struct mymsg {
    long mtype;
    char mtext[MAX_FILE_SIZE];
};

void countWords(char *);

int main(void) {

    key_t msgKey;
    int msgID;
    struct mymsg message;
    message.mtype = 1;

    if ((msgKey = ftok("/tmp", 'a')) < 0) {
        err("ftok", "cannot create msq key")
    }

    if ((msgID = msgget(msgKey, 0644)) < 0) {
        err("msgget", "cannot create/open msg")
    }

    if (msgrcv(msgID, &message, MAX_FILE_SIZE, message.mtype, 0) < 0) {
        err("msgrcv", "cannot receive message")
    }

    // puts flushes the buffer!

    countWords(message.mtext);

    if (msgctl(msgID, IPC_RMID, NULL) < 0) {
        perror("msgctl");
    }

    return 0;

}

void countWords(char *buffer) {

    int i, isLetter = 0, count = 0;
    while (buffer[i] != '\0') {
        switch (buffer[i]) {
            case '\t':
            case ' ':
            case '\n':
            case '\r':
            case '\0':
                isLetter = 0;
                break;
            default:
                if (isLetter == 0) {
                    count++;
                    isLetter = 1;
                }
                break;
        }
        i++;
    }

    printf("Word count: %d\n", count);

}
