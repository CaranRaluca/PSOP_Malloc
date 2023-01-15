#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


#include <dlfcn.h>

//#define _GNU_SOURCE

#include<string.h>


//#include "mymalloc.h"

int main(int argc, char **argv,char **env)
{
    int pid;
    pid=fork();
    int rc;
    
    if(pid>1)
    {
        printf("aaaa");
        int fd=open("valgrind",O_RDONLY);
        if(fd<0)
        {
            perror("fd");
            exit(-1);
        }
        int nr_bytes=lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);
        char buffer[nr_bytes];
        rc=read(fd,buffer,nr_bytes);
        if(rc==-1)
        {
            perror("read");
            exit(-1);
        }
        printf("%s",buffer);  
        close(fd);  
        //wait(NULL);
        //exit(0);    
    }
    
        if(pid==0)
    {

        printf("proces copil");
        rc=setenv("LD_PRELOAD","/home/caran_raluca/VSCode_PSO/PSOP/libmymalloc.so",1);
        
        char *path = getenv("LD_PRELOAD");
        char  pathenv[strlen(path) + sizeof("LD_PRELOAD=")];
        sprintf(pathenv, "LD_PRELOAD=%s", path);
        char *envp[] = {pathenv, NULL};
    //char *tests[] = {"ls", "-lR", NULL};
    //execvpe(tests[0], tests, envp);
    //fprintf(stderr, "failed to execute \"%s\"\n", tests[0]);



        char* argument_list[]={argv[1],NULL};
        //char *envp[]={env[0],NULL};
        execvpe(argv[1],argument_list,envp);
        //execvp(argv[1],argument_list);
        exit(0);
    }

    return 0;
}