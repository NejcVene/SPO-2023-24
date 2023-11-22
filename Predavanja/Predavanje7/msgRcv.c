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

    // tole more biti isto, da ga najde
    if ((key = ftok("/home", 'a')) < 0) {
        perror("ftok");
    }

    printf("Kluč je: %d\n", key);

    if ((msqID = msgget(key, 0644 | IPC_CREAT)) < 0) {
        perror("msgget");
    }

    // 0 -> če ni nobenega sporočila bo blokiral dokler ne dobi sporočila (izvajal se bo v prazno; čakal bo)
    // IPC_NOWAIT -> če ni nobenega sporočila, bo izpisal tole: "msgsnd: No message of desired type"

    if (msgrcv(msqID, &buffer, sizeof(buffer.mText), buffer.mType, IPC_NOWAIT) < 0) {
        perror("msgsnd");
    }

    puts(buffer.mText);

    if (msgctl(msqID, IPC_RMID, NULL) < 0) {
        perror("msgctl");
    }

    return 0;

}