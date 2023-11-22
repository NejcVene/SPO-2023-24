#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sem.h>

int main(int argc, char **argv) {

    key_t key;
    int semID, vrednost;
    struct sembuf operacije[1]; // katere operacije imamo nad semaforjem

    if (argc != 2) {
        printf("Uporaba: %s operacija (-1, 1, 0)", argv[0]);
        exit(1);
    }

    if ((key = ftok("/usr", 'q')) < 0) {
        perror("ftok");
    }

    if ((semID = semget(key, 1, 0644 | IPC_CREAT)) < 0) {
        perror("semget");
    }

    operacije[0].sem_num = 0; // številka semaforja je 0, ker je samo eden
    operacije[0].sem_op = atoi(argv[1]); // 1 -> inkrement (vračanje vira); -1 -> dekrement (zasedanje vira)
    operacije[0].sem_num = IPC_NOWAIT; // 0 pomeni, da blokira; IPC_NOWAIT pa ne blokira

    if (semop(semID, operacije, 1) < 0) {
        perror("semop");
    }
    
    // preberemo vrednost semaforja
    if ((vrednost = semctl(semID, 0, GETVAL)) < 0) {
        perror("semctl za vrednost");
    }
    printf("Vrednost semaforja je: %d\n", vrednost);

    return 0;

}