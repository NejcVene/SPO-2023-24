#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 265
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    int socketFD, readBytes;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // check for arguments
    if (argc < 2) {
        err("To few arguments.", "Usage: <port number>")
    }

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

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        printf("> ");
        // send user input to server
        if (write(socketFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
            err("Could not write to this socket", "write")
        }
        // read what server sends back
        readBytes = read(socketFD, buffer, BUFFER_SIZE);
        if (readBytes < 0) {
            err("Error", "err")
        }
        // print result
        buffer[readBytes - 1] = 0; 
        puts(buffer);
    }

    return 0;

}