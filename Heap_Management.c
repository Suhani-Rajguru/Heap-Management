/*BT23CSE055 SUHANI RAJGURU
CPL Assignment 2 */
//HEAP MANAGEMENT
#define SIZE_OF_HEAP 100
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h> 

typedef struct heap_tag
{
    char byte[SIZE_OF_HEAP];
    unsigned int lptr; 
} heap;

void init(heap *h)
{
    unsigned int *ptr;
    char *curr=h->byte;

    ptr=(unsigned int*)h->byte;

    for(int i=0;i<SIZE_OF_HEAP;i++)
    {
        h->byte[i]=0;
    } 
    *ptr=SIZE_OF_HEAP;
    
    curr=curr+4;
    ptr=(unsigned int*)curr;

    *ptr=101;

    h->lptr=0;
}

char* malloc_h(unsigned int size,heap *h)
{
    char *ptr=NULL;
    char *prev=NULL; 
    unsigned int *s;
    unsigned int *n; 
    unsigned int *ps;
    int flag=0; 
    size=size+8;

    if(h->lptr==101)
    {
        // Empty free list
        printf("\nMalloc Failure due to full heap\n");
        return NULL;
    }
    else{
        ptr=&(h->byte[h->lptr]);

        // First fit
        do
        {
            s=(unsigned int*)ptr; 
            if((*s)>=size)
            {
                flag=1; 
                n=(unsigned int*)(ptr+4);
            }
            else
            {
                prev=ptr;
                n=(unsigned int*)(ptr+4);
                if((*n)!=101)
                {
                    
                    ptr=&(h->byte[(*n)]);
                }
            }
        }while((*n!=101)&&(flag==0));
    }

    // Termination due to No block of sufficient size found
    if((*n==101)&&(flag==0))
    {
        // External Fragmentation
        printf("\nMalloc Failure due to External Fragmentation\n");
        return NULL;
    }
    else if((*n==101)&&(flag==1))
    {
        // Only one free block in heap
        // Set h->lptr 
        (h->lptr)=((ptr)-(h->byte))+size;
        ps=(unsigned int*)(&(h->byte[h->lptr]));
        *ps=(*s)-size;
        n=(unsigned int*)(&(h->byte[(h->lptr)+4]));
        *n=101; 
        *s=size; 
    }
    else{
        if(ptr-(h->byte)==h->lptr)
        {
            // First block in free list
            (h->lptr)=((ptr)-(h->byte))+size;

            ps=(unsigned int*)(h->byte+(h->lptr));
            *ps=(*s)-size;
            
            *s=size;
        }
    }
    
    return (ptr+8);
}

void free_h(char *ptr, heap *h)
{
    if (ptr == NULL)
        return;

    ptr -= 8; 
    unsigned int *size = (unsigned int*)ptr;
    unsigned int *next = (unsigned int*)(ptr + 4);

    *next = h->lptr;
    h->lptr = ptr - h->byte;

    // Merge adjacent free blocks
    char *curr = h->byte + h->lptr;
    while (curr < h->byte + SIZE_OF_HEAP)
    {
        unsigned int *curr_size = (unsigned int*)curr;
        unsigned int *curr_next = (unsigned int*)(curr + 4);

        if (*curr_next != 101 && curr + *curr_size == h->byte + *curr_next)
        {
            *curr_size += *((unsigned int*)(h->byte + *curr_next));
            *curr_next = *((unsigned int*)(h->byte + *curr_next + 4));
        }
        curr = h->byte + *curr_next;
    }
}

void main()
{
    heap h; // Created a virtual heap 
    init(&h); // Initialize the heap with all values set to 0 and metadata: size=100, next=101

    char *c;
    int *n;
    float *f;

    // Malloc char
    c=(char*)malloc_h(1,&h);
    if (c != NULL)
    {
        *c='v';
        printf("\nValue of char = %c",*c);
    }
    
    // Malloc int
    n=(int*)malloc_h(sizeof(int),&h);
    if (n != NULL)
    {
        *n=1000;
        printf("\nValue of integer = %d",*(n));
    }
    
    // Malloc float
    f=(float*)malloc_h(sizeof(float),&h);
    if (f != NULL)
    {
        *f=1.23;
        printf("\nValue of float = %f",*f);
    }

    // Free allocated memory
    free_h((char*)c, &h);
    free_h((char*)n, &h);
    free_h((char*)f, &h);
    
    // Allocate again to test merging
    n=(int*)malloc_h(sizeof(int),&h);
    if (n != NULL)
    {
        *n=5000;
        printf("\nAfter free, Value of integer = %d",*(n));  
    }

    printf("\nExpected garbage value of integer: %d\n", *(int*)f);
}
