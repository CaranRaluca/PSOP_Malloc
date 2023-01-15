#include "mymalloc.h"
//de modificat pe git

typedef struct CHUNK
{
    int available;   //1 zona este goala, 0 zona este ocupata
    chunk* next;
    int size;
    char end[1];

}chunk;

chunk* vector_heap[VECTOR_SIZE];
chunk* lastVisited = NULL;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* brkPoint0 = NULL;


typedef struct META_CHUNK
{
    void * address;
    int line;
}meta_chunk;

meta_chunk vector_meta_chunk[VECTOR_SIZE];

static int count_malloc=0;
static int count_free=0;

void garbage_collector();

static void CreateBin(int vector_index, int increase_value);

static void printList(chunk* headptr, int index);

static void printHeapStructure();

static void CreateHeapStructure();

static chunk *  NotEnoughSpaceManagement();

static void malloc_lines(void * address, int line);

static void free_lines(void *address);

static void AddChunkInOrder(chunk* head, chunk* new_chunk);

static void generate_file_output();



static void malloc_lines(void * address,int line)
{
    int i=0;
    while(vector_meta_chunk[i].line!=0)
        i++;
    vector_meta_chunk[i].address=address;
    printf("\n%p\n",address);
    vector_meta_chunk[i].line=line;
    count_malloc+=1;

    generate_file_output();
}

static void free_lines(void * address)
{
    int i=0;
    while(vector_meta_chunk[i].address!=address)
    {
        printf("%d",i);
        i++;
    }
    
    vector_meta_chunk[i].address=NULL;
    vector_meta_chunk[i].line=0;
    count_free+=1;
    
    generate_file_output();
}

void* mymalloc(int size,int line)
{
     printf("aaa");

    if (!brkPoint0)
    {
        CreateHeapStructure();
    }


    int index = 16;
    if (size > 512)
    {
        index = 520;
    }
    else
    {
        while (index < size)
        {
            index += 8;
        }
    }

    int index_vec_heap = index / 8 - 2;

    for (; index_vec_heap < VECTOR_SIZE - 1; index_vec_heap++)
    {
        chunk* head_chunk = vector_heap[index_vec_heap];
        chunk* p = head_chunk->next;
        while (p)
        {
            if (p->available==1)
            {
                malloc_lines(p->end,line);
                p->available = 0;
                printf("My malloc function at line: %d   %p\n",line,p->end);        
                return p->end;
            }
            p = p->next;
        }
    }

    // void* brkPoint1 = sbrk(0);

    // pthread_mutex_lock(&lock);
    // if (sbrk(size + sizeof(chunk)) == (void*)-1)		//incrementam varful heapului ; marim zona heap-ului 
    // {
    //     pthread_mutex_unlock(&lock);
    //     exit(-1);
    // }

    // chunk* new_chunk = brkPoint1;
    // new_chunk->size = size;
    // new_chunk->available = 0;

    // chunk* p = vector_heap[index_vec_heap];

    // while (p->next != NULL)
    // {
    //     p = p->next;
    // }
    // p->next = new_chunk;


    // return p->next->end;
    return NotEnoughSpaceManagement(size,line);

}

void myfree(void* ptr)
{
    chunk* ptr_free = ptr;
    int ok = 1;

    for (int index_vector_heap = 0; index_vector_heap < VECTOR_SIZE && ok == 1; index_vector_heap++)
    {
        chunk* head_chunk = vector_heap[index_vector_heap];
        chunk* p = head_chunk;
        while (p)
        {
            if (p->end == ptr)
            {
                printf("My free function.\n");
                p->available = 1;
                ok = 0;
                free_lines(ptr);
                break;
            }
            p = p->next;
        }
    }
  
    if (ok==1)
    {
        printf("Zona de memorie din afara heap-ului.\n");
        exit(-1);
    }

}

void garbage_collector()
{
    for (int index_vector_heap = 0; index_vector_heap < VECTOR_SIZE; index_vector_heap++)
    {
        chunk* head_chunk = vector_heap[index_vector_heap];
        chunk* p = head_chunk;
        while (p)
        {
            if (p->available == 0)
            {
                p->available = 1;
            }
            p = p->next;
        }
    }
}

static chunk* NotEnoughSpaceManagement(int size,int line){
  
    //chunk * head_final_bin = sbrk(0);
    chunk*  head_final_bin= vector_heap[VECTOR_SIZE-1];

    while (head_final_bin->next != NULL)
    {
        if(head_final_bin->available)
        {
            head_final_bin->available=0;
            malloc_lines(head_final_bin,line);
            return head_final_bin;
        }
        head_final_bin = head_final_bin->next;
    }

    void* brkPoint1 = sbrk(0);
    pthread_mutex_lock(&lock);
    if (sbrk(size + sizeof(chunk)) == (void*)-1)		//incrementam varful heapului ; marim zona heap-ului 
    {
        pthread_mutex_unlock(&lock);
        exit(-1);
    }

    chunk* new_chunk = brkPoint1;
    new_chunk->size = size;
    new_chunk->available = 0;

    chunk* p = vector_heap[VECTOR_SIZE-1];

    AddChunkInOrder(p,new_chunk);

    malloc_lines(p->next->end,line);
    return p->next->end;
 
}

static void AddChunkInOrder(chunk* head, chunk* new_chunk) {

    if (new_chunk == NULL || head == NULL)
        return;

    // head = head ->next;

    while (head->next != NULL)
    {
        if (new_chunk->size > head->next->size)
        {
            head->next = new_chunk;
            return;
        }
        head = head->next;

    }
    head->next = new_chunk;
}

static void CreateBin(int vector_index, int increase_value)
{
    chunk* head = vector_heap[vector_index];
    int count = 1;
    chunk* next_elem;
    chunk* curr_elem = head;

    //int size=NUMBER_OF_LARGE_CHUNKS * pow(2,VECTOR_SIZE-vector_index);
    int size = 10;

    while (size--)
    {
        chunk* new_chunk = lastVisited->end + increase_value;
        new_chunk->available = 1;
        new_chunk->size = increase_value;
        next_elem = new_chunk;
        lastVisited = new_chunk;
        curr_elem->next = next_elem;

        curr_elem = curr_elem->next;

        count++;
    }

}

static void printList(chunk* headptr, int index)
{
    int i = 0;
    chunk* p = headptr;
    printf("BINUL %d:\n", index + 1);
    while (p != NULL)
    {
        printf("[%d] p: %p\n", i, p);
        printf("[%d] p->size: %d\n", i, p->size);
        printf("[%d] p->available: %d\n", i, p->available);
        printf("[%d] p->next: %p\n", i, p->next);
        printf("\n\n");
        i++;
        p = p->next;
    }
}

static void printHeapStructure()
{
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        printList(vector_heap[i], i);
    }
}

static void CreateHeapStructure()
{
    void* brkPoint1;

    pthread_mutex_lock(&lock);
    brkPoint0 = sbrk(0);


    if (sbrk(HEAP_SIZE) == (void*)-1)		//incrementam varful heapului ; marim zona heap-ului 
    {
        pthread_mutex_unlock(&lock);
        exit(-1);
    }
    brkPoint1 = sbrk(0);
    //head=brkPoint0;

    int size = 16;
    ///int counter=log(1+HEAP_SIZE/NUMBER_OF_LARGE_CHUNKS)-1;

    int increment = 0;

    chunk* first_vec_element = brkPoint0;
    vector_heap[0] = first_vec_element;
    void* ptr = brkPoint0;

    //initializare vector
    int i = 1;
    for (; i < VECTOR_SIZE - 1; i++)
    {
        ptr = ptr + 20;
        vector_heap[i] = ptr;
    }


    lastVisited = vector_heap[i - 1];

    //construirea binurilor
    for (int index = 0; index < VECTOR_SIZE - 1; index++)
    {
        CreateBin(index, size);
        size += 8;
    }

    sbrk(sizeof(chunk));
    vector_heap[VECTOR_SIZE - 1] = brkPoint1;
    vector_heap[VECTOR_SIZE - 1]->next = NULL;
    pthread_mutex_unlock(&lock);

}

static void generate_file_output()
{
    /*for(int i=0;i<VECTOR_SIZE;i++)
    {
        if(vector_meta_chunk[i].line!=-1)
        printf("Address: %p  Line: %d\n",vector_meta_chunk[i].address,vector_meta_chunk[i].line);
        i++;
    } */
    FILE* f=fopen("valgrind","w");
    fprintf(f,"ERROR SUMMARY :\nmalloc/free:%d allocs, %d frees\n",count_malloc,count_free);
    fprintf (f,"\nMemory not freed:\nAddress\t Line\n");
    for(int i=0;i<VECTOR_SIZE;i++)
        if(vector_meta_chunk[i].line!=0)
            fprintf(f,"%p\t %d\n",vector_meta_chunk[i].address,vector_meta_chunk[i].line);
    
    fclose(f);
}