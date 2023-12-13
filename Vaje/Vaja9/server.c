#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 265
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    int listenFD, connectedFD, readBytes;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // check for arguments
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

    // get a socket
    if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Could not get a socket", socket)
    }

    // set serverAddr to 0
    bzero(&serverAddr, sizeof(serverAddr));

    // set server parameters
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(argv[1]);

    // set local address
    if (bind(listenFD, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("Could not bind local address", "bind")
    }

    // listen for clients (Prepare to accept connections on listenFD)
    if (listen(listenFD, SOMAXCONN) < 0) {
        err("Cannot listen for connections", "listen")
    }

    while (1) {
        // accept connection from queue
        if ((connectedFD = accept(listenFD, NULL, NULL)) < 0) {
            err("Could not accept conection", "accept")
        }
        // get/read whatever client sends to us
        while ((readBytes = read(connectedFD, buffer, BUFFER_SIZE)) < 0) {
            printf("Looking for %s in file %s", buffer, "server.c");
        }
        // close connection with client
        if (close(connectedFD) < 0) {
            err("Could not close connection with client", "close")
        }
    }

    // close server socket
    if (close(listenFD) < 0) {
        err("Could not close server socket", "close")
    }

    return 0;

}