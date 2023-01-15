#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define malloc(size) mymalloc(size,__LINE__)
#define free(address) myfree(address)

struct CHUNK;

typedef struct CHUNK chunk;

struct META_CHUNK;

typedef struct META_CHUNK meta_chunk;

#define NUMBER_OF_LARGE_CHUNKS 8
#define HEAP_SIZE 1000000000

#define STRUCT_SIZE sizeof(chunk)
#define VECTOR_SIZE 64

void print_lines();

void* mymalloc(int size,int line);

void myfree(void* ptr);