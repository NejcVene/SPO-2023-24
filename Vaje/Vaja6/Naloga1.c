#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define err(msg) { fprintf(stderr, "Error: %s\n", msg); exit(1); }
#define set_sig(sig, state) { if (signal(sig, state) == SIG_ERR) err("Signal error") }

void handlerSIGINT(int);
void handlerSIGALRM(int);

int setTime;

int main(void) {

    // set signal handlers for SIGINT and SIGALARM
    set_sig(SIGINT, handlerSIGINT)
    set_sig(SIGALRM, handlerSIGALRM)

    printf("Lovim signal SIGINT\n");

    setTime = 5;
    alarm(setTime);
    pause();
    pause();
    pause();

    return 0;

}

void handlerSIGINT(int sigNum) {

    // print how much seconds where left and set timer to it
    printf("Prejel sem signal %d\n", sigNum);
    int unsleptTime = alarm(0);
    printf("unslept_time = %d\n", unsleptTime);
    alarm(unsleptTime);
    pause();

}

void handlerSIGALRM(int sigNum) {

    // check to what time alarm was set
    // and change signal handler for SIGINT
    switch (setTime) {
        case 5: // 5 seconds pass -> now ignore SINGINT
            set_sig(SIGINT, SIG_IGN)
            setTime = 10;
            alarm(5);
            printf("Alarm v casu t = 5 ...\nIgnoriram signal SIGINT\n");
            break;
        case 10: // 10 seconds pass -> default action for SIGNINT
            set_sig(SIGINT, SIG_DFL)
            setTime = 15;
            alarm(5);
            printf("Alarm v casu t = 10 ...\nPrivzeta akcija za SINGINT\n");
            break;
        case 15: // 15 seconds pass -> no input was given. End the program
            printf("Alarm v casu t = 15 ...\n");
        default:
            break;
    }
}