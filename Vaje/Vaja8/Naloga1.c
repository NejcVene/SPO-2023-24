#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define Ps_MAX 1e9 // maximum iterations
#define P 1e6 // this is 1000000
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

// create a mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long Ps = 0, Zs = 0;
double calculatedSqrt = 0;

void *calculateSqrt(void *);

int main(int argc, char **argv) {

    // check that there are enough arguments
    if (argc < 2) {
        err("To few arguments.", "Usage: <number of threads>")
    }

    struct timespec timeStart, timeEnd;
    int numOfThreads = atoi(argv[1]);
    pthread_t threads[numOfThreads];
    double timeTaken;

    // start measuring time
    if (clock_gettime(CLOCK_REALTIME, &timeStart) < 0) {
        err("Could not start time", "clock_getime")
    }

    // create threads
    for (int i = 0; i<numOfThreads; i++) {
        if (pthread_create(
            &threads[i],
            NULL,
            calculateSqrt,
            NULL
        ) != 0) {
            err("Could not create a thread", "pthread_create")
        }
    }

    // wait for threads to finish
    for (int i = 0; i<numOfThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            err("Could not wait for threads", "pthread_join")
        }
    }

    // get end time
    if (clock_gettime(CLOCK_REALTIME, &timeEnd) < 0) {
        err("Could not start time", "clock_getime")
    }

    // calculate how long the program ran
    timeTaken = (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_nsec - timeStart.tv_nsec) / 1e9;

    // print results
    printf("sqrt(2) ≈ %f\nTime taken: %f seconds\n", calculatedSqrt, timeTaken);

    return 0;

}

// using Monte Carlo approach
void *calculateSqrt(void *arg) {

    struct timespec myTime;
    if (clock_gettime(CLOCK_REALTIME, &myTime) < 0) {
        err("Could not start time", "clock_getime")
    }
    unsigned int seed = (unsigned int) myTime.tv_nsec;
    double x, localZ;

    while (Ps <= Ps_MAX) {
        localZ = 0;
        // do P local interations
        for (int i = 0; i<P; i++) {
            // generate a random number between [0, 2) (including decimals)
            x = 2.0 * ((double) rand_r(&seed)) / RAND_MAX;
            if (x * x < 2) {
                localZ++;
            }
        }
        // critical section
        pthread_mutex_lock(&mutex);
        calculatedSqrt = 2 * localZ / P;
        Zs += localZ;
        Ps += P;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;

}