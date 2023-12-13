#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> // tole je za strukture
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <string.h>

#define LISTENQ 1024
#define BUFFER_SIZE 512
#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }

// daytime server bo vrnil čas

int main(int argc, char **argv) {

    int listenFD, connectFD, n, pid;
    struct sockaddr_in serverAddr; // naslov strežnika
    char buffer[BUFFER_SIZE];
    
    // check for arguments
    if (argc != 2) {
        err("To few arguments. Usage: <TCP port>")
    }

    // get a socket
    if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket")
    }

    bzero(&serverAddr, sizeof(serverAddr)); // tole nastavi vse na 0

    serverAddr.sin_family = AF_INET; // določi family
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // določi naslov
    serverAddr.sin_port = htons(atoi(argv[1])); // določi port

    // bind s vtičnico
    if (bind(listenFD, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        err("bind")
    }

    if (listen(listenFD, LISTENQ) < 0) {
        err("listen")
    }

    while (1) {
        connectFD = accept(listenFD, NULL, NULL); // če nas client addr ne zanima
        if (connectFD < 0) {
            err("accept")
        }

        // naredi otroka
        if ((pid = fork()) < 0) {
            err("fork")
        } else if (pid == 0) {
            // za otroka
            close(listenFD);
            while ((n = read(connectFD, buffer, BUFFER_SIZE)) > 0) {
                printf("Dobil sem: %s\n", buffer);
                // obdelava zahteve
                if (write(connectFD, buffer, strlen(buffer) + 1) != strlen(buffer) + 1) {
                    err("write")
                }
            }
            exit(0); // zaključi otroka
        }
        if (close(connectFD) < 0) {
            err("close connect")
        }
    }

    if (close(listenFD) < 0) {
        err("close listen")
    }

    return 0;

}