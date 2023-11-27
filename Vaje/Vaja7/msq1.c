#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MSG_SIZE 16 * 1000 // 16KB (max size to read from file)
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

struct myMsg {
    long mType;
    char mText[MSG_SIZE];
};

void readFile(char *, struct myMsg *);

int main(int argc, char **argv) {

    struct myMsg buffer;
    key_t key;
    int msqID;

    // check, if there are enough arguments
    if (argc < 2) {
        err("To few arumgents.", "Usage <filename>")
    }

    printf("Using file: %s\n", argv[1]);

    // set type and reaf file contents into buffer
    buffer.mType = 1;
    readFile(argv[1], &buffer);

    // generate a key
    if ((key = ftok("/home", 'a')) < 0) {
        err("Coudl not generate key", "ftok")
    }

    printf("Key %d (0x%x)\n", key, key);

    // create or open msg
    if ((msqID = msgget(key, 0644 | IPC_CREAT)) < 0) {
        err("Could not open or create msg", "msgget")
    }

    // send strint to msg
    if (msgsnd(msqID, &buffer, strlen(buffer.mText) + 1, 0) < 0) {
        err("Couldn not send string to msg", "msgsnd")
    }

    return 0;

}

// read from file
void readFile(char *fileName, struct myMsg *buffer) {
    
    int fd, n;
    if ((fd = open(fileName, O_RDONLY)) < 0) {
        err("Could not open file", fileName)
    }

    while ((n = read(fd, buffer->mText, MSG_SIZE)) > 0) {
        printf("Read bytes: %d\n", n);
    }

    if (close(fd) < 0) {
        err("Could not close file", fileName);
    }

}