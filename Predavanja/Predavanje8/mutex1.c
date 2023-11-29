#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int globalna;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
void *funkcija_niti(void *arg)
{
int i,j;
for ( i=0; i<10000; i++ ) {
pthread_mutex_lock(&mymutex);
j = globalna;
j++;
printf("b");
fflush(stdout);
// sleep(1);
globalna = j;
pthread_mutex_unlock(&mymutex);
}
return NULL;
}

int main()
{
pthread_t id;
int i;
if ( pthread_create( &id, NULL, funkcija_niti, NULL) ) {
printf("Napaka pri ustvarjanju niti.");
exit(1);
}
for ( i=0; i<10000; i++) {
pthread_mutex_lock(&mymutex);
globalna++;
pthread_mutex_unlock(&mymutex);
printf("a");
fflush(stdout);
// sleep(1);
}
if ( pthread_join(id, NULL)){
printf("Napaka pri zdruzevanju niti.");
exit(1);
}
printf("\nglobalna: %d\n", globalna); // v tem primeru bo 20
exit(0);
}

/*
Vidimo, da je globalna vrednost vedno enaka, in sicer 20000.
(ali 20, če grejo zanke do 10)
*/