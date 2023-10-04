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
int main(void) {

    int x = 5; // tale X je na skladu (kopica se uproabi v priermu dinamične alokacije pomnilnika)
    int *ptrX = &x;

    /*
    Vidimo, da ima kazalec drugo vrednost vedno, ko ga ponovno zaženemo.
    To je zaradi niti.
    */

    printf("x: %d, ptrX: %p *ptrX: %d\n", x, ptrX, *ptrX);

    *ptrX = 20;

    printf("x: %d, ptrX: %p *ptrX: %d\n", x, ptrX, *ptrX);

    printf("Size of int: %ld\n", sizeof(int));

    return 0;

}