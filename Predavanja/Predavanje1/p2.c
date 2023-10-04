#include <stdio.h>

/*
int main ali void main?
Običajno mamo int main, da vemo kakšen je izhodni status. Po 
default damo ponavadi kar return 0;
*/
/*
Format specifier v printf za integer je %d ali %i.
Delal bo isto.
Format za kazalec je %p.
Za HEX vrednost je %x ali %X
Float je %f
Niz je %s
Double je %lf

Dostopna dovoljenja ze nek file:
     u | g | o
    rwx|rwx|rwx
*/
/*
Shell ukaz lscpu ti pove kar dost informacij o računalniku.
Predvsem smo uporabili, da nam je povedal, če mamo little ali big endidan.
Ugotovili smo, da ima ta računalnik little endian.
*/
/*
sizeof() dejansko ni fukcija, ampak je macro, torej
lahko napišemo sizeof(int) ali sizeof int <- delal bo isto.
*/
/*
Pointer je naslov prvega bajta elementa.
*/
int main(void) {

    int v[3] = {10, 20, 30};
    int *ptr = v; // more vedit kakšnega tipa je kazalec, da potem ve za koliko ga povečat, če že bomo delali aritmetiko s njim

    printf("ptr: %d v[0]: %d\n", *ptr, v[0]);
    ptr++;
    printf("ptr: %d v[1]: %d\n", *ptr, v[1]);
    ptr++;
    printf("ptr: %d v[2]: %d\n", *ptr, v[2]);

    return 0;

}