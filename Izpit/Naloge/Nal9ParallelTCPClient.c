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

#define BUFFER_SIZE 256
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

// client
// 1. socket
// 2. connect

void childFunction(int);

int main(int argc, char **argv) {
 
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

    for (int i = 0; i<10; i++) {
        int socketFD;
        struct sockaddr_in sockAddr;
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(atoi(argv[1]));
        pid_t pid;

        if ((pid = fork()) < 0) {
            err("fork", "cannot create a process")
        } else if (pid == 0) {
            // child process
            if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                err("socket", "cannot create a socket")
            }

            if (inet_pton(AF_INET, "127.0.0.1", &sockAddr.sin_addr) < 0) {
                err("inet_pton", "cannot convert presentation to numeric")
            }

            if (connect(socketFD, (const struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0) {
                err("connect", "cannot connect to server")
            }

            childFunction(socketFD);

        }

    }

    // for parent process
    pid_t stopped;
    int status;
    for (int i = 0; i<10; i++) {
        if ((stopped = waitpid(-1, &status, 0)) < 0) {
            err("waitpid", "waitpid failed")
        }
        printf("Child %d stopped with %d\n", stopped, status);
    }


    return 0;

}

void childFunction(int socketFD) {

    char buffer[10];
    char rcvBuffer[BUFFER_SIZE];
    int readBytes;

    sprintf(buffer, "%d", getpid());

    printf("Sending: %s\n", buffer);
    if (write(socketFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
        err("write", "cannot write to server")
    }

    while ((readBytes = read(socketFD, rcvBuffer, BUFFER_SIZE)) > 0) {
        printf("%s\n", rcvBuffer);
    }

    exit(0);

}