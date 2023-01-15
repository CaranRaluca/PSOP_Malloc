#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define HEAP_CAPACITY 16000
#define STRUCT_SIZE 24 
#define ALIGN_SIZE 8
#define EXPAND 1000
#define ALIGN(size) (((size) + (ALIGN_SIZE-1)) & ~(ALIGN_SIZE-1))

typedef struct chunk
{
  int size;
  int available;  
  struct chunk* next;
  struct chunk* prev;
  char end[1]; 		//sfarsitul structurii
} chunk;

chunk *head = NULL;
chunk *lastVisited = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *brkPoint0 = NULL;

chunk* findChunk(chunk *headptr, unsigned int size);

void splitChunk(chunk* ptr, unsigned int size);


chunk* increaseAllocation(chunk *lastVisitedPtr, unsigned int size);

void mergeChunkPrev(chunk *freed);

void mergeChunkNext(chunk *freed);

void printList(chunk *headptr);

void *mymalloc(unsigned int _size) ;

unsigned int myfree(void *ptr);
	