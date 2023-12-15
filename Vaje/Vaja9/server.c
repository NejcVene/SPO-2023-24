#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

int main(int argc, char **argv) {

    int listenFD, connectedFD, readBytes;
    struct sockaddr_in serverAddr;
    char buffer[100],
         cmdResult[BUFFER_SIZE], 
         toClient[BUFFER_SIZE],
         command[BUFFER_SIZE];

    // check for arguments
    if (argc < 2) {
        err("To few arguments", "Usage: <port number>")
    }

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
        while ((readBytes = read(connectedFD, buffer, 100)) > 0) {
            if (strcmp(buffer, " ") == 0) {
                continue;
            }
            // get rid of new line char. (\n)
            buffer[strlen(buffer) - 1] = 0;
            // add user input to command string
            sprintf(command, "grep -n \"%s\" server.c | cut -d: -f1", buffer);
            printf("command: %s\n", command);

            // run command (make pipe, fork, exec, wait)
            FILE *runCommand = popen(command, "r");
            if (!runCommand) {
                err("Could not run command", "popen")
            }

            // get command result and append it sending buffer
            while (fgets(cmdResult, sizeof(cmdResult), runCommand)) {
                cmdResult[strlen(cmdResult) - 1] = 0;
                strcat(cmdResult, ",");
                strcat(toClient, cmdResult);
            }

            // try to close pipe 
            if (pclose(runCommand) < 0) {
                err("Could not close command", "pclose")
            }

            // if there was no result (no matching string)
            // append 0 to sending buffer
            // otherwise get rid of new line char. (\n)
            if (strlen(toClient) == 0) {
                strcat(toClient, "0");
            } else {
                toClient[strlen(toClient) - 1] = 0;
            }

            // send result to client
            if (write(connectedFD, toClient, strlen(toClient) + 1) != strlen(toClient) + 1) {
                err("Could not send to client", "write")
            }

            // clear sending buffer by writing 0 to it
            memset(toClient, 0, BUFFER_SIZE);
            if (toClient[0] != 0) {
                err("Could not reset buffer", "memset")
            }

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
