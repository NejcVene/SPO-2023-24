#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHARED_MEM_SIZE 1024
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int getSemVal(int);

int main(void) {

    int semID, memID;
    key_t key, mKey;
    char *mem;

    // key for semaphore
    if ((key = ftok("/home", 'd')) < 0) {
        err("ftok", "cannot generate key")
    }

    // get semaphore
    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        err("semget", "cannot create/get semaphore")
    }

    // shared memory below
    // key for memory
    if ((mKey = ftok("/home", 'e')) < 0) {
        err("ftok", "cannot generate key for shared memory")
    }

    // get memory
    if ((memID = shmget(mKey, SHARED_MEM_SIZE, 0644 | IPC_CREAT)) < 0) {
        err("shmget", "cannot create/get shared memory")
    }

    // attach yourself to memory when semaphore allows to do so
    while (getSemVal(semID) == 0);

    if ((mem = shmat(memID, (void *) 0, 0)) == (void *) -1) {
        err("shmat", "process cannot attach itself to shared memory")
    }

    printf("Memory contains: %s\n", mem);

    return 0;

}

int getSemVal(int semID) {

    int val;
    if ((val = semctl(semID, 0, GETVAL)) < 0) {
        err("semctl", "cannot get semaphore value")
    }

    printf("VAL: %d\n", val);
    return val;

}