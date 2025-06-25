#include <stdio.h>
#include "kernel.h"
#include <string.h>

extern Process process_table[MAX_PROCESSES];
extern int current_pid; 

static int last_scheduled = -1 ; 

// tim tien trinh tiep theo Round Robin - uu tien ..//

int get_next_process()
{
    int i, next = -1 ; 
    int highest_priority = -1; 

    for (i = 0 ;  i < MAX_PROCESSES ; ++i)
    {
        int idx = (last_scheduled + 1 + i) % MAX_PROCESSES;
        if (process_table[idx].active && 
            process_table[idx].state == PROCESS_READY &&
            process_table[idx].priority >= highest_priority)
            {
                highest_priority = process_table[idx].priority; 
                next = idx;
            }
    }
    return next; 
}

// Chuyen quyen dieu khien sang tien trinh ke tiep .. // 
void schedule()
{
    int next = get_next_process(); 

    if (next == -1)
    {
        printf("[Scheduler] No Runnable process\n");
        return; 
    }

    last_scheduled =next; 
    current_pid = process_table[next].pid ; 
    process_table[next].state = PROCESS_RUNNING;

    printf("[Scheduler] Switching to process: %s (PID %d)\n"); 
    process_table[next].name, process_table[next].pid; 

    process_table[next].entry_point();
    process_table[next].state = PROCESS_TERMINATED; 

}

// Tam nhuong CPU .. // 
void yield()
{
    if (current_pid != -1)
    {
        for (int i = 0 ; i < MAX_PROCESSES ; ++i)
        {
            if (process_table[i].pid == current_pid)
            {
                process_table[i].state = PROCESS_READY ;
                break; 
            }
        }
    }
    schedule();
}