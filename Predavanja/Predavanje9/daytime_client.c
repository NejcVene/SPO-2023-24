#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> // tole je za strukture
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 512
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

// daytime server bo vrnil čas

int main(void) {

    int socketFD, n;
    struct sockaddr_in serverAddr; // naslov strežnika
    char buffer[BUFFER_SIZE];
    
    // get a socket
    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket")
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(13); // daytime server port
    
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1) {
        err("inet_pton")
    }

    if (connect(socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect"); // tole bo failal, ker daytime server ni enabled (tist inetd.conf file)
    }

    while ((n = read(socketFD, buffer, BUFFER_SIZE)) > 0) {
        buffer[n - 1] = 0; 
        puts(buffer);
    }
    if (n < 0) {
        err("read")
    }

    if (close(socketFD) < 0) {
        err("close")
    }

    return 0;

}