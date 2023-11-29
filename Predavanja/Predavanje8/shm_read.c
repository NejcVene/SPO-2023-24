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

    // get key
    if ((key = ftok("/usr", 'q')) < 0) {
        perror("ftok");
    }

    // open a shared memory segment (size is 0, as it already exists)
    if ((shmID = shmget(key, 0, 0666 | IPC_CREAT)) < 0) {
        perror("shmget");
    }

    // attach to shared memory
    if ((addr = shmat(shmID, (void *) 0, 0)) == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // read whathever is in that mem. segment
    puts(addr);

    // detach from shared memory
    if (shmdt(addr) < 0) {
        perror("shmdt");
    }

    // remove the shared mem. segment
    if (shmctl(shmID, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(1);
    }

    return 0;

}

// ipcs -m (m kot memory)