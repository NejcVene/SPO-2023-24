#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void *threadFunction(void *);

int sum;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

struct threadArgs {
    int threadID;
};

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <number of threads>")
    }

    int numberOfThreads = atoi(argv[1]);
    pthread_t threads[numberOfThreads];
    struct threadArgs args[numberOfThreads];
    
    for (int i = 0; i<numberOfThreads; i++) {
        args[i].threadID = i;
        if (
            pthread_create(
                &threads[i],
                NULL,
                threadFunction,
                (void *) &args[i]
            ) != 0
        ) {
            err("pthread_create", "cannot create a thread")
        }
    }

    for (int i = 0; i<numberOfThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            err("pthread_join", "cannot sync threads")
        }
    }

    printf("SUM is: %d\n", sum);

    pthread_mutex_destroy(&myMutex);

    return 0;

}

void *threadFunction(void *args)  {

    struct threadArgs *i = (struct threadArgs *) args;
    printf("Pozdrav of niti %d\n", i->threadID);

    // critical section
    pthread_mutex_lock(&myMutex);
    sum += i->threadID;
    pthread_mutex_unlock(&myMutex);
    // end of critical section

    pthread_exit(NULL);

}