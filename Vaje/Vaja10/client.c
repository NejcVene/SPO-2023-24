#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 20
#define RECEIVE_BUFFER 512
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    // check for arguments
    if (argc < 2) {
        err("To few arguments.", "Usage: <port number>")
    }

    int status;

    // create 10 child processes
    for (int i = 0; i<10; i++) {
        int socketFD, readBytes;
        struct sockaddr_in serverAddr;
        char buffer[BUFFER_SIZE],
             receiveBuffer[RECEIVE_BUFFER];
        pid_t pid;

        // create a child
        if ((pid = fork()) < 0) {
            err("Could not create a child process", "fork")
        } else if (pid == 0) {
            // get a socket
            if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                err("Could not get a socket", "socket")
            }

            // set family and port
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(atoi(argv[1]));

            // convert from presentation format of an Internet number in buffer starting at CP to the binary network format 
            if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1) {
                err("Could not convert from presentation format to binary format", "inet_pton")
            }

            // open a connection on socket FD to peer at ADDR
            if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
                err("Could not open a connection on this socket", "connect")
            }

            // write PID into a string what is sent to the server
            sprintf(buffer, "%d", getpid());
            // send PID to server
            if (write(socketFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
                err("Could not write to this socket", "write")
            }
            // read what server sends back
            while ((readBytes = read(socketFD, receiveBuffer, RECEIVE_BUFFER)) > 0) {
                // print result
                receiveBuffer[readBytes - 1] = 0; 
                puts(receiveBuffer);
                exit(0);
            }
            if (readBytes < 0) {
                err("Could not receive from server", "read")
            }
        }
    }

    // main process waits for its children to finish
    for (int i = 0; i<10; i++) {
        waitpid(-1, &status, 0); // wait for any child process to finish
        printf("Status: %d\n", status);
    }

    return 0;

}