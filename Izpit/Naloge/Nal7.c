#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int asked = 0, answered = 0; // mutex for these two
time_t startTime;

struct threadArgs {
    int threadID;
    int semID;
    struct sembuf *semOps;
};

union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

void *threadFunction(void *);
void takeResource(int, struct sembuf *);
void releaseResource(int, struct sembuf *);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments", "Usage: <number of processes>")
    }

    int numberOfProc = atoi(argv[1]);
    pthread_t threads[numberOfProc];
    struct threadArgs threadArguments[numberOfProc];
    key_t key;
    int semID;
    union semun semUnion;
    semUnion.val = 1;
    struct sembuf semOps[1];

    if ((key = ftok("/home", 'c')) < 0) {
        err("ftok", "cannot generate key")
    }

    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        err("semget", "cannot create semaphore")
    }

    if (semctl(semID, 0, SETVAL, semUnion) < 0) {
        err("semctl", "cannot set semaphore inital value")
    }

    semOps[0].sem_flg = 0;
    semOps[0].sem_num = 0;

    time(&startTime);

    for (int i = 0; i<numberOfProc; i++) {
        threadArguments[i].threadID = i;
        threadArguments[i].semID = semID;
        threadArguments[i].semOps = semOps;
        if (
            pthread_create(
                &threads[i],
                NULL,
                threadFunction,
                (void *) &threadArguments[i]
            ) < 0
        ) {
            err("pthread_create", "cannot create thread/threads")
        }
    }

    for (int i = 0; i<numberOfProc; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            err("pthread_join", "cannot wait for thread")
        }
    }

    pthread_mutex_destroy(&myMutex);

    return 0;

}

void *threadFunction(void *args) {

    struct threadArgs *val = (struct threadArgs *) args;
    time_t currentTime;

    time(&currentTime);
    printf("Kupec %d prišel (t=%ld)\n", val->threadID, currentTime - startTime);

    takeResource(val->semID, val->semOps);

    sleep(3);
    time(&currentTime);
    printf("Kupec %d vprašal (t=%ld)\n", val->threadID, currentTime - startTime);
    sleep(5);
    time(&currentTime);
    printf("Prodajalec odgovoril (t=%ld)\n", currentTime - startTime);
    
    releaseResource(val->semID, val->semOps);

    pthread_exit(NULL);

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