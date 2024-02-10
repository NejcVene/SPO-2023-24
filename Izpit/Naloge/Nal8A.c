#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHARED_MEM_SIZE 1024
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

void takeResource(int, struct sembuf *);
void releaseResource(int, struct sembuf *);

int main(void) {

    pid_t pid = getpid();
    int semID, memID;
    key_t key, mKey;
    struct sembuf semOps[1];
    union semun semUnion;
    char *mem;
    char buffer[SHARED_MEM_SIZE];

    semOps[0].sem_flg = 0;
    semOps[0].sem_num = 0;

    semUnion.val = 1;

    if ((key = ftok("/home", 'd')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        err("semget", "cannot create/get semaphore")
    }

    if (semctl(semID, 0, SETVAL, semUnion) < 0) {
        err("semctl", "cannot set inital value for semphore")
    }

    // take semaphore
    takeResource(semID, semOps);

    // shared memory below
    if ((mKey = ftok("/home", 'e')) < 0) {
        err("ftok", "cannot generate key for shared memory")
    }

    if ((memID = shmget(mKey, SHARED_MEM_SIZE, 0644 | IPC_CREAT)) < 0) {
        err("shmget", "cannot create/get shared memory")
    }

    if ((mem = shmat(memID, (void *) 0, 0)) == (void *) -1) {
        err("shmat", "process cannot attach itself to shared memory")
    }

    // write pid of process to shared memory
    sprintf(buffer, "pid = %d", pid);
    printf("Writing: %s\n", buffer);
    strcpy(mem, buffer);

    sleep(10);

    // release semaphore
    releaseResource(semID, semOps);

    return 0;

}

void takeResource(int semID, struct sembuf *semOps) {

    semOps[0].sem_op = -1;
    if (semop(semID, semOps, 1) < 0) {
        err("semop", "cannot take resource")
    }

}

void releaseResource(int semID, struct sembuf *semOps) {

    semOps[0].sem_op = 1;
    if (semop(semID, semOps, 1) < 0) {
        err("semop", "cannot release resource")
    }

}