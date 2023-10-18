#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }
int main(void)
{
if (open("tempfile", O_RDWR) < 0)
err("open error")
if (unlink("tempfile") < 0)
err("unlink error")
printf("file unlinked\n");
sleep(15);
printf("done\n");
return 0;
/*
če sem prav razumel, če bi že direktno zapri datoteko
po unlink, nam ne bi bilo potrebno čakati 15 sekund.
*/
} 