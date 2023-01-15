#include "myfirstfit.h"

chunk* findChunk(chunk *headptr, unsigned int size)
{
  chunk* ptr = headptr;
  
  while(ptr != NULL)
  {
    if(ptr->size >= (size + STRUCT_SIZE) && ptr->available == 1)
    {
      return ptr;
    }
    lastVisited = ptr;
    ptr = ptr->next;
  }  
  return ptr;  
}

//divide un chunk in 2 : primul chunk are dimensiunea ceruta , al doilea dimensiunea ramasa
void splitChunk(chunk* ptr, unsigned int size)
{
  chunk *newChunk;	
  
  newChunk = ptr->end + size;
  newChunk->size = ptr->size - size - STRUCT_SIZE;
  newChunk->available = 1;
  newChunk->next = ptr->next;
  ptr->next = newChunk;
  newChunk->prev = ptr;
  
   if((newChunk->next) != NULL)
   {      
      (newChunk->next)->prev = newChunk;
   }
  
  ptr->size = size+STRUCT_SIZE;
  ptr->available = 0;
}

chunk* increaseAllocation(chunk *lastVisitedPtr, unsigned int size)
{
  brkPoint0 = sbrk(0);
  chunk* curBreak = brkPoint0;		//brk curent
  
  if(sbrk(EXPAND) == (void*) -1)
  {
    return NULL;
  }
  
  curBreak->size = EXPAND;
  curBreak->available = 0;
  curBreak->next = NULL;
  curBreak->prev = lastVisitedPtr;
  lastVisitedPtr->next = curBreak;
  
  if(curBreak->size > size)
    splitChunk(curBreak, size);
  
  return curBreak;  
}

void mergeChunkPrev(chunk *freed)
{ 
  chunk *prev;
  prev = freed->prev;
  
  if(prev != NULL && prev->available == 1)
  {
    prev->size = prev->size + freed->size -STRUCT_SIZE;
    prev->next = freed->next;
    if( (freed->next) != NULL )
      (freed->next)->prev = prev;
  }
}

void mergeChunkNext(chunk *freed)
{  
  chunk *next;
  next = freed->next;
  
  if(next != NULL && next->available == 1)
  {
    freed->size = freed->size + next->size-STRUCT_SIZE;
    freed->next = next->next;
    if( (next->next) != NULL )
      (next->next)->prev = freed;
  }
}

void printList(chunk *headptr)
{
int i = 0;
  chunk *p = headptr;
  
  do
  {
    printf("[%d] p: %p\n", i, p);
    printf("[%d] p->size: %d\n", i, p->size);
    printf("[%d] p->available: %d\n", i, p->available);
    printf("[%d] p->prev: %p\n", i, p->prev);
    printf("[%d] p->next: %p\n", i, p->next);
    printf("\n\n");
    i++;
    p = p->next;
  }while(p != NULL);
}

void *mymalloc(unsigned int _size) 
{
  //pthread_mutex_lock(&lock);
  
  
  void *brkPoint1;
  unsigned int size = ALIGN(_size);
  chunk *ptr;
  chunk *newChunk;

  pthread_mutex_lock(&lock);
  brkPoint0 = sbrk(0);
  // sbrk() with an increment of 0 can be used to find the current  locationof the program break.
  //varful stivei

  if(head == NULL)				
  {
    if(sbrk(HEAP_CAPACITY) == (void*) -1)		//incrementam varful heapului ; marim zona heap-ului 
    {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    
    //Cream primul chunk cu dimensiunea egala cu a intregului heap
    brkPoint1 = sbrk(0);   
    head = brkPoint0;
    head->size = HEAP_CAPACITY;
    head->available = 0;
    head->next = NULL;
    head->prev = NULL;

    ptr = head;
    //divizam primul chunk in 2: unul de dimensiunea specificata de parametrul size si unul de dimensiunea ramasa 
    splitChunk(ptr, size);

    pthread_mutex_unlock(&lock);
    
    return ptr->end;
  }
  
  else								
  {
    chunk *freeChunk = NULL;
    freeChunk = findChunk(head, size);
    
    if(freeChunk == NULL)					//nu exista loc liber in memeorie de dimensiunea ceruta
    {
      freeChunk = increaseAllocation(lastVisited, size);	//marim heap-ul
      if(freeChunk == NULL) 					//Couldn't extend heap. increaseAllocation returned NULL (sbrk error)
      {
	      pthread_mutex_unlock(&lock);
	      return NULL;
      }
      pthread_mutex_unlock(&lock);
      return freeChunk->end;
    }
    
    else						//A chunk was found
    {
      if(freeChunk->size > size+STRUCT_SIZE*2)			//Daca chunk-ul este prea mare il impartim in 2 chunk-uri. 
	      splitChunk(freeChunk, size);
    }    
    pthread_mutex_unlock(&lock);   
    freeChunk->available=0; 
    return freeChunk->end;
  }  
}

unsigned int myfree(void *ptr) {
	pthread_mutex_lock(&lock);
	
	chunk *toFree;
	toFree = ptr - STRUCT_SIZE;

  if(toFree->available==1)
  {
    printf("Error:Nu poti dezaloca un obiect nealocat.");
    exit(-1);
  }
	
	if(toFree >= head && toFree <= brkPoint0)
	{
	  toFree->available = 1;	
	  mergeChunkNext(toFree);
	  mergeChunkPrev(toFree);
	  pthread_mutex_unlock(&lock);
	  return 0;
	  
	}
	else
	{
	  pthread_mutex_unlock(&lock);
	  return 1;
	}
}