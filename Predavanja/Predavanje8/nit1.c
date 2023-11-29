#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

void *funkcijaNiti(void *arg) {

    printf("Pozdrav od niti\n");

    return (void *) 4; // kar kol lahka vrne

}

int main(void) {

    pthread_t tid;
    void *retval; // za pthdread_join

    if (pthread_create(
        &tid,
        NULL,
        funkcijaNiti,
        NULL
    ) != 0) {
        err("pthread_create")
    }

    if (pthread_join(tid, &retval) != 0) {
        err("pthread_join")
    }

    printf("Rerval: %ld\n", (long) retval); // treba pretvorit (recimo v int ali long; ta dva se dost uporabljata)

    return 0;

}