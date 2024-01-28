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

void readFromFile(char *, struct mymsg*);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <file_name>")
    }

    key_t msgKey;
    int msgID;
    struct mymsg message;

    if ((msgKey = ftok("/tmp", 'a')) < 0) {
        err("ftok", "cannot create msq key")
    }

    if ((msgID = msgget(msgKey, 0644 | IPC_CREAT)) < 0) {
        err("msgget", "cannot create/open msg")
    }

    readFromFile(argv[1], &message);
    message.mtype = 1;

    if (msgsnd(msgID, &message, strlen(message.mtext) + 1, 0) < 0) {
        err("msgsend", "cannot send message")
    }

    return 0;

}

void readFromFile(char *fileName, struct mymsg *message) {

    int fd, readBytes;
    if ((fd = open(fileName, O_RDONLY)) < 0) {
        err("open", "cannot open file")
    }

    while ((readBytes = read(fd, message->mtext, MAX_FILE_SIZE)) > 0) {
        printf("Read bytes: %d\n", readBytes);
    }

    if (readBytes < 0) {
        err("read", "cannot read from file")
    }

    if (close(fd) < 0) {
        err("close", "cannot close file")
    }

}
