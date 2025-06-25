#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 

#include "kernel.h"

#define MEMORY_SIZE 1024 *1024 
#define MAX_SEGMENTS 128 

typedef struct 
{   
    int segment_id ;
    size_t start;
    size_t size ; 
    bool used ; 
    int owner_pid; 
} MemorySegment; 

static char memory_pool[MEMORY_SIZE];
static MemorySegment segments[MAX_SEGMENTS]; 
static int segment_counter = 0 ; 

// KHoi tao bo nho .. // 
void memory_init()
{
    memset(memory_pool , 0, MEMORY_SIZE); 
    memset(segments, 0 , sizeof(segments));
    segment_counter = 0 ; 
    printf("[Memory] Initialized memory pool (1mB)\n"); 
}

// cap phat bo nho .. / / 
void *allocate_memory(int pid, size_t size)
{
    if (segment_counter >= MAX_SEGMENTS)
    return NULL;  

    size_t offset = 0; 

    for (int i = 0; i < segment_counter ; ++i)
    {
        offset = segments[i].start + segments[i].size; 
    }

    if (offset + size > MEMORY_SIZE) return NULL;

    segments[segment_counter] = (MemorySegment)
    {
        .segment_id = segment_counter, 
        .start = offset,
        .size = size, 
        .used = 1, 
        .owner_pid = pid
    }; 

    segment_counter ++;  

    printf("[Memory] Allocated %zu bytes to PID %d at offset %zu\n", size, pid, offset);
    return &memory_pool[offset];
}

// Giai phong bo nho..// 
void free_memory(int pid){
    for (int i = 0 ; i < segment_counter ; ++i)
    {
        if (segments[i].owner_pid == pid && segments[i].used)
        {
            segments[i].used = 0;
            printf("[Memory] Freed segments %d from PID %d\n", segments[i].segment_id, pid);
        }
    }
}