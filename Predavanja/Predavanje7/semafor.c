#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sem.h>


union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};


int main(void) {

    union semun arg;
    key_t key;
    int semID;

    if ((key = ftok("/usr", 'q')) < 0) {
        perror("ftok");
    }

    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        perror("semget");
    }

    arg.val = 0;
    if (semctl(semID, 0, SETVAL, arg) < 0) {
        perror("semctl");
    }

    

    return 0;

}