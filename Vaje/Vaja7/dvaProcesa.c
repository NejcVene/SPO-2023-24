#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIM_PROC 2
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

time_t startTime;

void processFunction();
void incrementSem(int, struct sembuf *);
void decrementSem(int, struct sembuf *);
void getSemVal(int);

int main(int argc, char **argv) {

    // check if there are enough arguments
    if (argc < 2) {
        err("To few arguments.", "Usage <number of proceses>")
    }

    pid_t pid;
    union semun semArgs;
    struct sembuf semOps[1];
    int numberOfProc = atoi(argv[1]), semID, status;
    key_t key;

    // create a key
    if ((key = ftok("/tmp", 'A')) < 0) {
        err("Could not create key", "ftok")
    }

    // create or open a semaphore
    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        err("Could not create or open semaphore", "semget")
    }

    // set how many resources are available
    semArgs.val = SIM_PROC;
    // save the above value to the semaphore
    if (semctl(semID, 0, SETVAL, semArgs) < 0) {
        err("Could not set semaphore inital value", "semctl")
    }

    semOps[0].sem_num = 0; // how many operations
    semOps[0].sem_flg = 0; // set blocking mode (if no resources are available, then the process will wait)

    // start counting time
    time(&startTime);

    // create N processes and if possble start their function
    for (int i = 0; i<numberOfProc; i++) {
        if ((pid = fork()) < 0) {
            err("Could not create a child process", "fork")
        }
        // if child process
        if (pid == 0) {
            // take one resource if possible
            decrementSem(semID, semOps);
            // start its function
            processFunction();
            // free one resource
            incrementSem(semID, semOps);
            // exit with status 0
            exit(0);
        }
    }

    // main process waits for its children to finish
    for (int i = 0; i<numberOfProc; i++) {
        waitpid(-1, &status, 0); // wait for any child process to finish
    }

    return 0;

}

// function to get current semaphore value
void getSemVal(int semID) {

    int val;
    if ((val = semctl(semID, 0, GETVAL)) < 0) {
        err("Could not get value of semaphore", "semctl")
    }

    printf("Semaphore value: %d\n", val);

}

// function to free one resource
void incrementSem(int semID, struct sembuf *semOps) {

    semOps[0].sem_op = 1; // set operation. 1 -> increment; -1 -> decrement
    if (semop(semID, semOps, 1) < 0) {
        err("Could not increment (return resource)", "semop")
    }

}

// function to take one resource
void decrementSem(int semID, struct sembuf *semOps) {

    semOps[0].sem_op = -1; // set operation. 1 -> increment; -1 -> decrement
    if (semop(semID, semOps, 1) < 0) {
        err("Could not decrement (take resource)", "semop")
    }

}

// the function that each process runs (except main)
// it sleeps for 5 seconds
// and prints the time when it started and ended
void processFunction() {

    time_t currentTime;
    time(&currentTime);
    printf("Proces %d zacne. (t=%ld)\n", getpid(), currentTime - startTime);
    sleep(5);
    time(&currentTime);
    printf("Proces %d konca. (t=%ld)\n", getpid(), currentTime - startTime);

}
