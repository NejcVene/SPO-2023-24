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

    // count the number of words that are in the msg.
    printf("Counting words for: %s\n", buffer.mText);
    countWords(buffer.mText);

    return 0;

}

// count number of words in the buffer
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