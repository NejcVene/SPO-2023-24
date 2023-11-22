#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct myMsg {
    long mType;
    char mText[256];
};

int main(void) {

    struct myMsg buffer;
    key_t key;
    int msqID;

    if ((key = ftok("/home", 'a')) < 0) {
        perror("ftok");
    }

    printf("Kluč je: %d\n", key);

    if ((msqID = msgget(key, 0644 | IPC_CREAT)) < 0) {
        perror("msgget");
    }

    strcpy(buffer.mText, "Sporočilo v sporočilno vrsto.");

    buffer.mType = 8;

    if (msgsnd(msqID, &buffer, strlen(buffer.mText) + 1, 0) < 0) {
        perror("msgsnd");
    }

    return 0;

}

/*
ipcs (ipc status) -q (kot vrsta) 
*/