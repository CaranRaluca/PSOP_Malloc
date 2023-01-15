#include <stdlib.h>
#include <stdio.h>
#include "myfirstfit.h"

int main()
{
    //cum marim heapu-ul 
    int *a = (int *)mymalloc(4*sizeof(int));
    char *b= (char *)mymalloc(3*sizeof(char));
    printList(head);
    myfree(a);
    printList(head);
    a=NULL;
    return 0;
}

//gcc test.c -g3 -o test -lmyfirstfit -L .
