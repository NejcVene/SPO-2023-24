#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <string.h>

#define LISTENQ 1024
#define BUFFER_SIZE 1024
#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }

/*
    Napišite dva programa: odjemalec in strežnik, ki komunicirata na osnovi vtičnic.
        • Odjemalec pošlje strežniku določen niz (primera testnih nizov: ”ER_MODE” in “LEVEL1”).
        • Strežnik pogleda in javi odjemalcu, v katerih vrsticah datoteke /etc/init/gdm.conf se navedeni niz nahaja.
        Npr. “3, 7, 12” ali “0”, če ga ni.
        • Program, ki samo ugotovi, ali se dani niz nahaja v gornji datoteki, ali ne, se smatra kot delna rešitev naloge
            in se točkuje z največ 70%.
        • Uporaba ukazov grep, strcmp, strncmp, strstr ni dovoljena
    
    Files: Nal1Server.c, Nal1Client.c
*/

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

void serverFunction(int);
void searchFile(char *, char *, int);
int compareStrings(char *, char *);
int strtokParse(char *command, char *delimiter, char *parsedCommand[]);

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
        serverFunction(clientSocket);
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

void serverFunction(int clientFD) {

    char buffer[BUFFER_SIZE];
    char resultBuffer[BUFFER_SIZE];
    int readBytes;
    // get file string to search
    while ((readBytes = read(clientFD, buffer, BUFFER_SIZE)) > 0) {
        printf("%s", buffer);
        buffer[strlen(buffer) - 1] = '\0';
        searchFile(buffer, resultBuffer, BUFFER_SIZE);
        memset(buffer, 0, BUFFER_SIZE);
        printf("\n\n%s\n", resultBuffer);

        if (write(clientFD, resultBuffer, strlen(resultBuffer)) != strlen(resultBuffer)) {
            err("write", "cannot write so client")
        }
        memset(resultBuffer, 0, BUFFER_SIZE);
    }
    

}

void searchFile(char *toSearch, char *result, int size) {

    int lineCount = 0, index;
    char buffer[BUFFER_SIZE];
    char convertBuffer[BUFFER_SIZE];
    char *parsedCommand[BUFFER_SIZE];
    FILE *f;
    
    if (!(f = fopen("prog1.c", "r"))) {
        err("fopen", "cannot open file")
    }

    while (fgets(buffer, BUFFER_SIZE, f) != NULL) {
        lineCount++;
        index = strtokParse(buffer, " ", parsedCommand);
        for (int i = 0; i<index; i++) {
            if (compareStrings(toSearch, parsedCommand[i]) == 1) {
                printf("LINE: %d\n", lineCount);
                sprintf(convertBuffer, "%d ", lineCount);
                strcat(result, convertBuffer);
            }
        }
    }

    result[strlen(result) - 1] = '\0';

    if (strlen(result) == 0) {
        strcat(result, "0");
    }

    if (fclose(f) == EOF) {
        err("fclose", "cannot close file")
    }

}

int compareStrings(char *s1, char *s2) {

    // printf("%s and %s\n", s1, s2);

    int s1Len = strlen(s1);
    int s2Len = strlen(s2);

    if (s1Len != s2Len) {
        return 0;
    }

    for (int i = 0; i<s1Len; i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }

    return 1;

}

int strtokParse(char *command, char *delimiter, char *parsedCommand[]) {

    int index = 0;
    char *token = strtok(command, delimiter);
    while (token != NULL) {
        // save returned ponter into an array of char pointers
        parsedCommand[index++] = token;
        token = strtok(NULL, delimiter);
    }

    // return how many values are there in the char array
    return index;

}