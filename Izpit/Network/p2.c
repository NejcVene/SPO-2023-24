#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int getSocket();
void connetSocket(int, int);
void closeSocket(int);

// client
// 1. socket
// 2. connet

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <port number>")
    }

    int clientSocket = getSocket();
    connetSocket(clientSocket, atoi(argv[1]));
    closeSocket(clientSocket);

    return 0;

}

int getSocket() {

    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket", "cannot create a socket")
    }

    return socketFd;
 
}

void connetSocket(int socketFD, int port) {

    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, "127.0.0.1", &clientAddr.sin_addr) < 0) {
        err("inet_pton", "cannot convert presentation to numeric")
    }
    if (connect(socketFD, (const struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0) {
        err("connect", "cannot connet to server")
    }

}

void closeSocket(int socketFD) {

    if (close(socketFD) < 0) {
        err("close", "cannot close socket")
    }

}