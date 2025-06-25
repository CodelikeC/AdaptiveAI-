#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "kernel.h"

#define MAX_PROCESS 128 

typedef enum 
{
    PROCESS_READY, 
    PROCESS_RUNNING, 
    PROCESS_WAITING, 
    PROCESS_TERMINATED

} ProcessState;

typedef struct 
{
    int pid ; 
    char name [ 32]; 
    ProcessState state; 
    void (*entry_point) (void); 
} ProcessControlBlock;

static ProcessControlBlock process_table[MAX_PROCESSES];
static int next_pid = 1 ; 
static int current_pid = - 1; 

void init_kernel()
{
    memset(process_table, 0, sizeof(process_table)); 
    printf("Kernel, Initialized process table");

}

int spawn_process(const char *name, void (*entry_point)(void) )
{
    for (int i = 0 ; i < MAX_PROCESSES ; ++i)
    {
        if (process_table[i].state == PROCESS_TERMINATED || process_table[i].pid == 0){
            process_table[i]. pid = next_pid ++;
            strncpy(process_table[i].name, name, sizeof(process_table)); 
            process_table[i].state = PROCESS_READY;
            process_table[i].entry_point = entry_point; 
            printf("Kernel - Spawned process %s(PID = %d)", name, process_table[i].pid) ;
            return process_table[i].pid;
        }
         
    }

    printf("Kernel - Failed to spawn process : max limit"); 
    return - 1 ; 
}

void terminate_process(int pid){
    for (int i = 0 ; i < MAX_PROCESSES ; ++i)
    {
        if (process_table[i].pid == pid)
        {
            process_table[i].state = PROCESS_TERMINATED ; 
            printf("Kernel - Terminate process : %d\n" , process_table[i].pid);
            return; 
        }
    }
}


void run_kernel()
{
    while (1){
        for (int  i = 0 ; i< MAX_PROCESSES ; ++i)
        {
            if(process_table[i].state == PROCESS_READY)
            {
                current_pid = process_table[i].pid ; 
                process_table[i].state = PROCESS_RUNNING; 
                printf("Kernel - Running PID = %d (%s) \n", current_pid ) ;
                process_table[i].entry_point(); 
                process_table[i].state  = PROCESS_WAITING; 
            }
        }
    }
}