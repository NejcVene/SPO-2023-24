#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
en ukaz
dva ukaza -> pipe:
    treba označit kje je prvi in drugi ukaz -> treba preprečit, da kreira še enega otroka in cev
    treba ločit argumente za prvi in drugi ukaz
*/

#define err(msg, reason) { fprintf(stderr, "Error: %s %s\n", msg, reason); exit(1); }
#define INPUT_LENGTH 1024
#define ARGUMENT_LENGTH 64

int parse(char [], char *[]);
int strtokParse(char *, char *, char *[]);

int main(void) {

    char inputBuffer[INPUT_LENGTH];
    char *parsedCommand[ARGUMENT_LENGTH],
         *firstArgs[ARGUMENT_LENGTH],
         *secondArgs[ARGUMENT_LENGTH];
    int status,
        numberOfArgs, 
        count = 0, 
        secondCommand;
    int pfd[2];
    pid_t pidChild, 
          pidGrandchild;
    
    // loop till user inputs EOF (CTRL + D)
    while (fgets(inputBuffer, INPUT_LENGTH, stdin) != NULL) {
        secondCommand = -1;
        pidGrandchild = -1;
        // increment and chekc if count is even or odd
        if ((++count) % 2 == 0) {
            printf("Using: Strtok\n");
            // if it is even, then user strtok
            numberOfArgs = strtokParse(inputBuffer, " ", parsedCommand);
        } else {
            printf("Using: Parse\n");
            // if it is odd, then use custom parse
            numberOfArgs = parse(inputBuffer, parsedCommand);
        }

        // find start of second command
        for (int i = 0; i<numberOfArgs; i++) {
            if (strcmp(parsedCommand[i], "==>") == 0) {
                secondCommand = ++i;
            }
        }

        // if a second command was found, then split arguments into two arrays, that are then used by the two commands
        // firstArgs are arguments for the first command
        // secondArgs are argumetns for the second command
        if (secondCommand > 0) {
            // used as a counter to set the correct index for secondArgs
            int index = 0;
            for (int i = 0; i<numberOfArgs; i++) {
                if (i < secondCommand - 1) {
                    firstArgs[i] = parsedCommand[i];
                } else if (i >= secondCommand) {
                    secondArgs[index++] = parsedCommand[i];
                }
            }
            // set last elements of both arrays to NULL
            firstArgs[secondCommand - 1] = NULL;
            secondArgs[index] = NULL;
            // get rid of '\n' for last string in secondArgs
            secondArgs[index - 1][strlen(secondArgs[index - 1]) - 1] = '\0';
            // try to create a pipe
            if (pipe(pfd) < 0) {
                err("Could not create pipe", "pipe")
            }
        } else {
            // if no second command was foud, then procede as usual (only one command)
            parsedCommand[numberOfArgs] = NULL;
            parsedCommand[numberOfArgs - 1][strlen(parsedCommand[numberOfArgs - 1]) - 1] = '\0';
        }

        // try to create a new child
        if ((pidChild = vfork()) < 0) {
            err("Could not create child process", "fork")
        }

        if (pidChild == 0) { // for child process

            // if there is a second command, try to create a second child
            if (secondCommand > 0 && (pidGrandchild = vfork()) < 0) {
                err("Could not create child process", "fork")
            }

            // for the second child (only used when there are two command)
            if (pidGrandchild == 0) {
                // close write
                close(pfd[1]);
                // dupe read to stdin
                dup2(pfd[0], STDIN_FILENO);
                // close read as it is not needed anymore
                close(pfd[0]);
                // execute the second command
                if (execvp(secondArgs[0], secondArgs) == -1) {
                    err("Could not run exec2", parsedCommand[secondCommand]) // if an error occures, then use exit(1) or whatever exit the command gives, if it failes
                }
                exit(0);
            } else if (pidChild == 0) { // for the first child
                // this branch is used when there is one or two commands
                // check if there are two commands
                if (secondCommand > 0) {
                    // close read
                    close(pfd[0]);
                    // dupe write to stdout
                    dup2(pfd[1], STDOUT_FILENO);
                }
                // run command (and use correct arguments)
                if (execvp(parsedCommand[0], secondCommand > 0 ? firstArgs : parsedCommand) == -1) {
                    err("Could not run exec1", parsedCommand[0]) // if an error occures, then use exit(1) or whatever exit the command gives, if it failes
                }
                // close write if there are two commands
                if (secondCommand > 0) {
                    close(pfd[1]);
                }

                exit(0); // for normal operation, return/exit with 0
            }
        
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

// parse both spaces and \t
int parse(char command[], char *args[]) {

    int commandIndex = 0, argsIndex = 0, isEmpty = 0;
    while (command[commandIndex] != '\0') {
        if (command[commandIndex] == ' ' || command[commandIndex] == '\t') {
            command[commandIndex] = '\0';
            isEmpty = 0;
        } else {
            if (isEmpty == 0) {
                args[argsIndex++] = &command[commandIndex];
                isEmpty = 1;
            }
        }
        commandIndex++;
    }

    return argsIndex;

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