#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

int main(void) {

    key_t key;
    int shmID;
    char *addr;
    char buffer[] = "Vpis v deljeni pomnilnik.";

    // create key
    if ((key = ftok("/usr", 'q')) < 0) {
        perror("ftok");
    }

    // create a shared memory segment
    if ((shmID = shmget(key, 512, 0666 | IPC_CREAT)) < 0) {
        perror("shmget");
    }

    // attach to shared memory
    if ((addr = shmat(shmID, (void *) 0, 0)) == (void *) -1) {
        perror("shmat");
    }

    // write string to shared memory
    strcpy(addr, buffer);

    // detach from shared memory
    if (shmdt(addr) < 0) {
        perror("shmdt");
    }

    return 0;

}

// ipcs -m (m kot memory)