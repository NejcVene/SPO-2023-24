#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define err_sys(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }
#define PAGER "more"
#define MAXLINE 256
int main(int argc, char *argv[])
{
char line[MAXLINE];
FILE *fpin, *fpout;
if (argc != 2) {
printf("usage: a.out <pathname>"); exit(1);
}
if ( (fpin = fopen(argv[1], "r")) == NULL)
err_sys("cannot open the file")
if ( (fpout = popen(PAGER, "w")) == NULL)
err_sys("popen")
/* copy argv[1] to pager */
while (fgets(line, MAXLINE, fpin) != NULL) {
if (fputs(line, fpout) == EOF)
err_sys("fputs")
}
if (ferror(fpin))
err_sys("fgets")
if (pclose(fpout) == -1)
err_sys("pclose")
exit(0);
}
