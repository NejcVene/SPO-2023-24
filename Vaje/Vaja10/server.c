#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 20
#define SENDING_BUFFER 512
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

void sigchldHander(int);
void childFunction(int, struct sockaddr_storage);

int main(int argc, char **argv) {

    int listenFD, connectedFD, readBytes;
    pid_t pid;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage clientAddr;
    socklen_t addrLen;

    // check for arguments
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

    // set signal handler for SIGCHLD
    signal(SIGCHLD, sigchldHander);

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

    while(1) {
        // accept connection from queue
        if ((connectedFD = accept(listenFD, (struct sockaddr *) &clientAddr, &addrLen)) < 0) {
            err("Could not accept conection", "accept")
        }
        // create a child process
        if ((pid = fork()) < 0) {
            err("Could not create a child process", "fork")
        } else if (pid == 0) { // for child process
            close(listenFD);
            // call child function
            childFunction(connectedFD, clientAddr);
            exit(0);
        } else { // parent
            // close connection with client
            if (close(connectedFD) < 0) {
                err("Could not close connection with client", "close")
            }
        }
    }

    // close server socket
    if (close(listenFD) < 0) {
        err("Could not close server socket", "close")
    }

    return 0;

}

// send this signal to parent when child process ends
// using waitpid with WNOHANG captures more children
void sigchldHander(int sigNum) {

    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("PID %d Status: %d\n", pid, status);
    }

    if (pid == -1 && errno != ECHILD) {
        err("It is not no child process(10)", "waitpid")
    }

}

void childFunction(int connectedFD, struct sockaddr_storage clientAddr) {

    int readBytes;
    char clientIP[INET_ADDRSTRLEN],
         buffer[100],
         toClient[SENDING_BUFFER];
    struct sockaddr_in *clientInfo = (struct sockaddr_in *) &clientAddr;

    while ((readBytes = read(connectedFD, buffer, BUFFER_SIZE))) {
        // convert a internet address in binary network format to presentation form
        inet_ntop(AF_INET, &clientInfo->sin_addr, clientIP, sizeof(clientIP));
        // write what is going to be sent to client into a string
        sprintf(toClient, "Streznik otrok %d sem stregel odjemalcu (%s:%d) otroku %s",
            getpid(),
            clientIP,
            ntohs(clientInfo->sin_port),
            buffer);
        printf("%s\n", toClient);
        // send result to client
        if (write(connectedFD, toClient, strlen(toClient) + 1) != strlen(toClient) + 1) {
            err("Could not send to client", "write")
        }
    }

}
