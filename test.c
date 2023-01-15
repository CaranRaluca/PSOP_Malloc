//#include "mymalloc.c"
#include <stdlib.h>
#include <stdio.h> 

int main(int argc, char **argv,char **env)
{
    //printf("aaa");
    int* ptr = malloc(4*sizeof(int));
    int* ptr1=malloc(5);
    char *c=malloc(5*sizeof(char));
    free(ptr1);
}

//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
//LD_PRELOAD=/home/caran_raluca/VSCode_PSO/PSOP/libmymalloc.so /bin/ls
//gcc test.c -g3 -o test -lmymalloc -L .