#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define NUM_OF_THREADS 10
#define BUFFER_SIZE    20
#define RECEIVE_BUFFER 512

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

struct threadArgs {
    int threadID,
        port;
};

void *threadFunction(void *);

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <port number>")
    }

    pthread_t threads[NUM_OF_THREADS];

    // create threads
    for (int i = 0; i<NUM_OF_THREADS; i++) {
        struct threadArgs *threadArgs = malloc(sizeof(struct threadArgs));
        threadArgs->threadID = i;
        threadArgs->port = atoi(argv[1]);

        if (pthread_create(
            &threads[i], 
            NULL, 
            threadFunction, 
            (void *) threadArgs
        ) != 0) {
            err("Could not create threads", "pthread_create")
        }
    }

    // wait for threads to finish
    for (int i = 0; i<NUM_OF_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            err("Could not wait for threads", "pthread_join")
        }
    }

    return 0;
}

void *threadFunction(void *args) {

    struct threadArgs *threadArgs = (struct threadArgs *) args;

    int socketFD, readBytes;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE],
         receiveBuffer[RECEIVE_BUFFER];

    // get a socket
    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Could not get a socket", "socket")
    }

    // set family and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(threadArgs->port);

    // convert from presentation format of an Internet number in buffer starting at CP to the binary network format 
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1) {
        err("Could not convert from presentation format to binary format", "inet_pton")
    }

    // open a connection on socket FD to peer at ADDR
    if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("Could not open a connection on this socket", "connect")
    }

    // communicate with the server
    sprintf(buffer, "%d", threadArgs->threadID);
    // send thread ID to server
    if (write(socketFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
        err("Could not write to this socket", "write")
    }

    // read what server sends back
    if ((readBytes = read(socketFD, receiveBuffer, RECEIVE_BUFFER)) > 0) {
        // print result
        receiveBuffer[readBytes - 1] = 0; 
        puts(receiveBuffer);
    }

    if (readBytes < 0) {
        err("Could not receive from server", "read")
    }

    // close the client socket
    close(socketFD);

    free(args);

    return NULL;

}