#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

// server
// 1. socket
// 2. bind
// 3. listen
// 4. accept

void childFunction(int, struct sockaddr_in *);
void sigchldHander(int);

int main(int argc, char **argv) {
 
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

    int socketFD;
    int connectedFD;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t len;
    serverAddr.sin_port = htons(atoi(argv[1]));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    pid_t pid;

    bzero(&serverAddr, sizeof(serverAddr));

    if (signal(SIGCHLD, sigchldHander) == SIG_ERR) {
        err("signal", "cannot assign signal handler")
    }

    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket", "cannot create a socket")
    }

    if (bind(socketFD, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("bind", "cannot bind socket to address")
    }

    if (listen(socketFD, SOMAXCONN) < 0) {
        err("listen", "cannot listen on this port/address")
    }

    while (1) {
        if ((connectedFD = accept(socketFD, NULL, NULL)) < 0) {
            err("accept", "cannot accept inbound connection")
        }

        if ((pid = fork()) < 0) {
            err("fork", "cannot create child process")
        } else if (pid == 0) {
            // child process
            close(socketFD);
            childFunction(connectedFD, &clientAddr);
        } else {
            if (close(connectedFD) < 0) {
                err("close", "cannot close connection")
            }
        }

    }

    return 0;

}

void childFunction(int socketFD, struct sockaddr_in *clientAddr) {

    char rcvBuffer[10];
    char sendBuffer[BUFFER_SIZE];
    char client[INET_ADDRSTRLEN];
    int readBytes;
    strcpy(sendBuffer, "fuck you");
    
    while ((readBytes = read(socketFD, rcvBuffer, 10)) > 0) {


        if (write(socketFD, sendBuffer, strlen(sendBuffer) + 1) != strlen(sendBuffer) + 1) {
            err("write", "cannot write to client")
        }

    }

    exit(0);

}

void sigchldHander(int sigNum) {

    printf("Have signal: %d\n", sigNum);

    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("PID %d Status: %d\n", pid, status);
    }

    if (pid == -1 && errno != ECHILD) {
        err("It is not no child process(10)", "waitpid")
    }

}
