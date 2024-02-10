#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#define LISTENQ 1024
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

// server

// 1. socket
// 2. bind
// 3. listen
// 4. accept

int getSocket();
void bindSocket(int, int);
void listenSocket(int);
int acceptSocket(int);
void closeSocket(int);
void sigINTHandler(int);

int serverSocket;

int main(int argc, char **argv) {

    if (argc < 2) {
        err("To few arguments.", "Usage: <port number>")
    }

    if (signal(SIGINT, sigINTHandler) == SIG_ERR) {
        err("signal", "cannot assing signal handler for SIGINT")
    } 

    int clientSocket;
    serverSocket = getSocket();

    bindSocket(serverSocket, atoi(argv[1]));

    listenSocket(serverSocket);

    while (1) {
        clientSocket = acceptSocket(serverSocket);
        closeSocket(clientSocket);
    }

    return 0;

}

int getSocket() {

    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket", "cannot create a socket")
    }

    return socketFd;
 
}

void bindSocket(int socketFD, int port) {

    struct sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    if (bind(socketFD, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("bind", "cannot give socketFD local address")
    }

}

void listenSocket(int socketFD) {

    if (listen(socketFD, LISTENQ) < 0) {
        err("listen", "cannot listen on provided socket")
    }

}

int acceptSocket(int socketFD) {

    int clientFD, clientPort;
    struct sockaddr_in clientInfo;
    socklen_t socketLen;
    char buffer[1024];
    if ((clientFD = accept(socketFD, (struct sockaddr *) &clientInfo, &socketLen)) < 0) {
        err("accept", "cannot accept inbound connection")
    }

    if (inet_ntop(AF_INET, &clientInfo.sin_addr, buffer, sizeof(buffer)) == NULL) {
        err("inet_ntop", "cannot convert numeric to presentation")
    }

    if ((clientPort = ntohs(clientInfo.sin_port)) < 0) {
        err("ntohs", "cannot convert between network and host byte order")
    }

    printf("Client %s connected on %d\n", buffer, clientPort);

    return clientFD;

}

void closeSocket(int socketFD) {

    if (close(socketFD) < 0) {
        err("close", "cannot close socket")
    }

}

void sigINTHandler(int sigNum) {

    printf("Recevied signal %d\nClosing server socket\n", sigNum);
    closeSocket(serverSocket);

}