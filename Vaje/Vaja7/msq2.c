#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 16 * 1000 // 16KB (max size to read from file)
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void countWords(char *);

struct myMsg {
    long mType;
    char mText[MSG_SIZE];
};

int main(void) {

    struct myMsg buffer;
    key_t key;
    int msqID;

    // generate a key
    if ((key = ftok("/home", 'a')) < 0) {
        err("Coudl find key", "ftok")
    }

    printf("Key %d (0x%x)\n", key, key);

    // open msg
    if ((msqID = msgget(key, 0644)) < 0) {
        err("Could not open msg", "msgget")
    }

    // receive from msg
    if (msgrcv(msqID, &buffer, MSG_SIZE, buffer.mType, 0) < 0) {
        err("Could not receive msg", "msgrcv")
    }

    printf("Counting words for: %s\n", buffer.mText);
    countWords(buffer.mText);

    return 0;

}

void countWords(char *buffer) {

    int i, isEmtpy = 0, count = 0;
    while (buffer[i] != '\0') {
        switch (buffer[i]) {
            case '\t':
            case ' ':
            case '\n':
            case '\r':
            case '\0':
                isEmtpy = 0;
                break;
            default:
                if (isEmtpy == 0) {
                    count++;
                    isEmtpy = 1;
                }
                break;
        }
        i++;
    }

    printf("Word count: %d\n", count);

}