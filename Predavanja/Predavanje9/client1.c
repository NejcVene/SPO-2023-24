#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> // tole je za strukture
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 512
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

// daytime server bo vrnil čas

int main(int argc, char **argv) {

    int socketFD, n;
    struct sockaddr_in serverAddr; // naslov strežnika
    char buffer[BUFFER_SIZE];

    if (argc != 2) {
        err("To few arguments. Usage: <TCP port>")
    }
    
    // get a socket
    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket")
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1])); // določi port
    
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1) {
        err("inet_pton")
    }

    if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect");
    }

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (write(socketFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
            err("write")
        }
        while ((n = read(socketFD, buffer, BUFFER_SIZE)) > 0) {
            buffer[n - 1] = 0; 
            puts(buffer);
        }
    }
    if (n < 0) {
        err("read")
    }

    if (close(socketFD) < 0) {
        err("close")
    }

    return 0;

}