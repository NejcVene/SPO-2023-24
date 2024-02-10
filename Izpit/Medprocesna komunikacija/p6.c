#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

int main(int argc, char **argv) {

    union semun semUnion;
    semUnion.val = 10;

    if (argc < 2) {
        err("To few arguments.", "Usage <-1, 1>")
    }

    key_t key;
    if ((key = ftok("/", 'a')) < 0) {
        err("ftok", "cannot generate key")
    }

    int semID;
    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        err("semget", "cannot open/create semaphore")
    }

    /*
    if (semctl(semID, 0, SETVAL, semUnion) < 0) {
        err("semctl", "cannot set value of 10 for sempahore")
    }
    */

    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = atoi(argv[1]);
    operations[0].sem_flg = 0;

    if (semop(semID, operations, 1) < 0) {
        err("semop", "cannot run operation")
    }

    int value;
    if ((value = semctl(semID, 0, GETVAL)) < 0) {
        err("semctl", "cannot get semaphore value")
    }

    printf("Semaphore value: %d\n", value);

    return 0;

}