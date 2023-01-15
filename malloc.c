#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


void *mymalloc(size_t size) {
  void *p = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  } else {
    assert(p == request); // Not thread safe.
    return p;
  }
}

int main()
{
  int *p=(int*)malloc(8*sizeof(int));
  int *b=(int*)malloc(sizeof(int));
  char *c=(char*)malloc(1);
  char *d=(char*)malloc(1);

}
