#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THR 5
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

void *funkcijaNiti(void *arg) {

    printf("Pozdrav od niti %ld\n", (long) arg);

    return (void *) 0; // kar kol lahka vrne

}

int main(void) {

    pthread_t tid[N_THR];
    void *retval; // za pthdread_join

    for (int i = 0; i<N_THR; i++) {
        if (pthread_create(
            &tid[i],
            NULL,
            funkcijaNiti,
            (void *) ((long) i) // kot ID niti
        ) != 0) {
            err("pthread_create")
        }
    }
    
    for (int i = 0; i<N_THR; i++) {
        if (pthread_join(tid[i], &retval) != 0) {
            err("pthread_join")
        }
    }

    printf("Retval: %ld\n", (long) retval); // treba pretvorit (recimo v int ali long; ta dva se dost uporabljata)

    return 0;

}