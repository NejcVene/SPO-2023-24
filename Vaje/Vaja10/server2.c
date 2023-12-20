#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define NUM_OF_THREADS 10
#define BUFFER_SIZE    20
#define SENDING_BUFFER 512

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct threadArgs {
    int threadID,
        connectedFD;
    struct sockaddr_storage clientAddr;
};

void *threadFunction(void *);

int main(int argc, char **argv) {

    // check for arguments
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

    int listenFD, connectedFD;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage clientAddr;
    pthread_t threads[NUM_OF_THREADS];
    socklen_t addrLen;

    // get a socket
    if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Could not get a socket", "socket")
    }

    // set serverAddr to 0
    bzero(&serverAddr, sizeof(serverAddr));

    // set server parameters
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    // set local address
    if (bind(listenFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("Could not bind local address", "bind")
    }

    // listen for clients (Prepare to accept connections on listenFD)
    if (listen(listenFD, SOMAXCONN) < 0) {
        err("Cannot listen for connections", "listen")
    }

    // Accept client connections and handle each one in a separate thread
    for (int i = 0; i<NUM_OF_THREADS; i++) {
        if ((connectedFD = accept(listenFD, (struct sockaddr *) &clientAddr, &addrLen)) < 0) {
            err("Could not accept conection", "accept")
        }

        // create arguments structure for the thread
        struct threadArgs *threadArgs = malloc(sizeof(struct threadArgs));
        threadArgs->threadID = i;
        threadArgs->connectedFD = connectedFD;
        threadArgs->clientAddr = clientAddr;

        // create a thread for each client connection
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
            err("Could not detach threads", "pthread_detach")
        }
    }

    // close server socket
    close(listenFD);

    // the main thread has to keep running to allow the threads to finish and clean up
    while (1);

    return 0;
}

void *threadFunction(void *args) {

    struct threadArgs *threadArgs = (struct threadArgs *) args;

    int readBytes;
    char clientIP[INET_ADDRSTRLEN],
         buffer[100],
         toClient[SENDING_BUFFER];
    struct sockaddr_in *clientInfo = (struct sockaddr_in *) &threadArgs->clientAddr;

    // Receive data from the client
    if ((readBytes = read(threadArgs->connectedFD, buffer, BUFFER_SIZE)) > -1) {

        // convert a internet address in binary network format to presentation form
        inet_ntop(AF_INET, &clientInfo->sin_addr, clientIP, sizeof(clientIP));
        // write what is going to be sent to client into a string
        sprintf(toClient, "Streznik otrok %d sem stregel odjemalcu (%s:%d) otroku %s",
            threadArgs->threadID,
            clientIP,
            ntohs(clientInfo->sin_port),
            buffer);
        printf("%s\n", toClient);
        // send result to client
        if (write(threadArgs->connectedFD, toClient, strlen(toClient) + 1) != strlen(toClient) + 1) {
            err("Could not send to client", "write")
        }
    }

    // Close the client socket and free the arguments structure
    close(threadArgs->connectedFD);

    free(args);

    return NULL;
}
