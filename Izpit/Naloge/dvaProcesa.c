#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define NUMBER_OF_RES 2
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

union semun {
    int val; // za SETVAL
    struct semid_ds *buf;
    unsigned short *array;
};

void processFunction();
void incrementSem(int, struct sembuf*);
void decrementSem(int, struct sembuf*);
void getSemVal(int);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <number of proc>")
    }

    key_t semKey;
    pid_t pid, pidChild;
    int semID, status;
    int numProc = atoi(argv[1]);
    union semun semUnion;
    struct sembuf buf[1];

    semUnion.val = NUMBER_OF_RES;

    if ((semKey = ftok("/tmp", 'A')) < 0) {
        err("ftok", "cannot create key")
    }

    if ((semID = semget(semKey, 1, 0644 | IPC_CREAT)) < 0) {
        err("semget", "cannot get a semaphore")
    }

    if (semctl(semID, 0, SETVAL, semUnion) < 0) {
        err("semctl", "cannot set semaphore value")
    }

    buf[0].sem_num = 0;
    buf[0].sem_flg = 0;

    for (int i = 0; i<numProc; i++) {
        if ((pid = fork()) < 0) {
            err("fork", "cannot create a process")
        } else if (pid == 0) {
            // child
            decrementSem(semID, buf);
            processFunction();
            incrementSem(semID, buf);
            exit(0);
        }
    }

    for (int i = 0; i<numProc; i++) {
        if ((pidChild = waitpid(-1, &status, 0)) > 0) {
            printf("Child with PID of %d exited with %d\n", pidChild, status);
        }
        if (pidChild == -1 && errno != ECHILD) {
            err("It is not no child process(10)", "waitpid")
        }
    }

    return 0;

}

void getSemVal(int semID) {

    int val;
    if ((val = semctl(semID, 0, GETVAL)) < 0) {
        err("semctl", "cannot get semaphore value")
    }

    printf("Semaphore value: %d\n", val);

}

void decrementSem(int semID, struct sembuf *sem) {

    sem->sem_op = -1;
    if (semop(semID, sem, 1) < 0) {
        err("semop", "cannot decrement semaphore")
    }

}

void incrementSem(int semID, struct sembuf *sem) {

    sem->sem_op = 1;
    if (semop(semID, sem, 1) < 0) {
        err("semop", "cannot increment semaphore")
    }

}

void processFunction() {

    printf("PID %d to sleep\n", getpid());
    sleep(5);
    printf("PID %d to awake\n", getpid());

}