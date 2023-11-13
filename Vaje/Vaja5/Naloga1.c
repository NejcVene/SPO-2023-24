#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }
#define INPUT_LENGTH 1024
#define ARGUMENT_LENGTH 64

int parse(char [], char *[]);
int strtokParse(char *, char *, char *[]);

int main(void) {

    char inputBuffer[INPUT_LENGTH];
    char *parsedCommand[ARGUMENT_LENGTH];
    int status, numberOfArgs, count = 0;
    pid_t pid;
    
    // loop till user inputs EOF (CTRL + D)
    while (fgets(inputBuffer, INPUT_LENGTH, stdin) != NULL) {
        // increment and chekc if count is even or odd
        if ((++count) % 2 == 0) {
            // if it is even, then user strtok
            numberOfArgs = strtokParse(inputBuffer, " ", parsedCommand);
        } else {
            // if it is odd, then use custom parse
            numberOfArgs = parse(inputBuffer, parsedCommand);
        }

        // make sure that the last element is NULL and there is no '\n' at the last string
        parsedCommand[numberOfArgs] = NULL;
        parsedCommand[numberOfArgs - 1][strlen(parsedCommand[numberOfArgs - 1]) - 1] = '\0';

        // try to create a new child
        if ((pid = vfork()) < 0) {
            err("Could not create child process", "fork")
        }
        if (pid == 0) { // for child process
            // try to run the inputed command
            if (execvp(parsedCommand[0], parsedCommand) == -1) {
                err("Could not run exec", parsedCommand[0]) // if an error occures, then use exit(1) or whatever exit the command gives, if it failes
            }
            _exit(0); // for normal operation, return/exit with 0
        } else { // for parent process
            // wait for child process to finish and then pritn its exit status
            wait(&status);
            if (WIFEXITED(status)) {
                printf("Child exit status: %d\n\n", WEXITSTATUS(status));
            }
        }

    }

    return 0;

}

int parse(char command[], char *args[]) {

    int index = 0;
    args[index++] = command; // get the pointer to the first string
    for (int i = 0; i<strlen(command); i++) {
        // if it is an whitespace, then save the next character's memory location into char pointer array
        // it saves the next character location, not whitespaces'
        if (command[i] == ' ') {
            args[index++] = &command[i + 1];
        }
    }

    // if there is only one string, dont run this
    if (index > 1) {
        // if there is more than one, replace every whitespace with '\0'
        for (int i = 0; i<index; i++) {
            *(args[i] - 1) = '\0';
        }
    }
    // make the last elelment NULL
    args[index] = NULL;

    //return how many elements there are
    return index;

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