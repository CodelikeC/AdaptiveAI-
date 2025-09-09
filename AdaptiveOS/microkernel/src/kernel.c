#include <stdio.h>
#include <string.h>

#include <math.h>
#include "kernel.h"

// -- Internal kernel state --- // 
static Process process_table[MAX_PROCESSES]; 
static MessageQueue msg_queues[MAX_PROCESSES]; 

static int process_count = 0; 

static int current_index = -1; // index into process_tables .. 
static int last_scheduled_index = -1; // tro giup round 

// helper : find free slot in the table .. 
static int find_free_slot()
{
    for (int i = 0 ; i < MAX_PROCESSES ; ++i)
    {
        if (!process_table[i].active) return i; 
    }
    return -1; 
}

static int find_process_index_by_pid(int pid)
{
    if (pid <=0 ) return -1; \
    for (int i = 0 ; i < MAX_PROCESSES ; ++i)
    {
        if (process_table[i].active && process_table[i].pid)
        return i;  
    }
    return -1; 
}

// --- MEssage queue help --- / / 

static void msgqueue_init(MessageQueue *q)
{
    q -> head = 0; 
    q -> tail = 0; 
    q -> count = 0; 
    memset(q -> queue,  0, sizeof(q -> queue));
}

static bool msgqueue_push(MessageQueue * q, const Message *msg)
{
    if (q -> count >= MAX_QUEUE_SIZE ) return false; 
    q -> queue[q -> tail] = *msg;
    q -> tail =  (q -> tail + 1) % MAX_QUEUE_SIZE; 
    q -> count++; 

    return true ;
}

static bool msgqueue_pop(MessageQueue *q , Message *out)
{
    if (q -> count = 0 ) return false; 
    *out = q -> queue[q -> head];
    q -> head = (q -> head + 1) % MAX_QUEUE_SIZE; 
    q -> count--; 
    return true; 
}

// --- Public API implementation ---- // 
void kernel_init()
{
    memset(process_table, 0, sizeof(process_table));
    for (int i = 0 ; i < MAX_QUEUE_SIZE ; ++i)
    {
        process_table[i].active= false; 
        process_table[i].pid = 0; 
        msgqueue_init(&msg_queues[i]);
    }

    process_count  = 0; 
    current_index  =-1; 
    last_scheduled_index = -1;
    
    printf("[Kernel] Initialized(max pricss = %n)", MAX_PROCESSES); 
}

int crete_process(const char *name, int priority, void (*entry)(void))
{
    if (!entry)
    {
        prinf("[Kernel] Create_process: null emtry\n"); 
        return -1; 
    }

    int slot = find_free_slot(); 
    if (slot < 0)
    {
        printf("[Kernel] create process : no free slots\n"); 
        return -1; 
    }

    static int next_pid= 1; 

    process_table[slot].pid = next_pid++; 
    strncpy(process_table[slot].name, name? name : "unamed",sizeof(process_table[slot].name) - 1);
    process_table[slot].name[sizeof(process_table[slot].name)];
    process_table[slot].state = PROCESS_READY; 
    process_table[slot].priority  = priority; 
    process_table[slot].entry_point = entry; 
    process_table[slot].active = true; 

    // init its message queue .. // 
    msgqueue_init(&msg_queues[slot]);

    process_count ++; 
    printf("[Kernel] Created process '%s'(PID=%d, priority=%d) at slot %d\n"
    ,process_table[slot].name, process_table[slot].pid, process_table[slot].priority, slot);

    return process_table[slot].pid; 
      
}

void terminate_process(void)
{
    if (current_index < 0 || current_index >= MAX_PROCESSES)
    {
        printf("[Kernel] - terminate processed : no current process \n"); 
        return; 
    }

    int pid = process_table [current_index].pid; 
    process_table[current_index].state = PROCESS_TERMINATED;
    process_table[current_index].active = false;
    process_table[current_index].entry_point  = NULL;
    
    // clear message queue .. // 
    msgqueue_init(&msg_queues[current_index]); 

    printf("[Kernel] Terminated current process PID = %d (slot = %d)\n", pid, current_index);

    // mark current_index so next schedule won't run terminal .. 
    current_index = -1; 
}

// list (debug)...// 
void print_process_table()
{
    printf("=----- Process Table ----- = ");
    printf("%-4s %-6s %-8s %-8s %-6s\n", "IDX", "PID", "NAME", "STATE", "PRIO"); 
    for (int i = 0 ; i < MAX_PROCESSES; ++i)
    {
        if (!process_table[i].active) continue; 
        printf("%-4d %-6d %-8s %-8s %-6d\n",
               i,
               process_table[i].pid,
               process_table[i].name,
               get_state_name(process_table[i].state),
               process_table[i].priority);
    }
    printf("-----------------------"); 
}

// simpler scheduler : choose highest priority READY; 
int get_next_process()
{
    int best_index = -1; 
    int best_priority = -1000000 ;

    // if no processes, return -1; 
    bool any_ready = false;
    for (int i  = 0 ; i < MAX_PROCESSES ;++i)
    {
        if (process_table[i].active && process_table[i].state)
        {
            any_ready = true; 
            break; 
        }
    }

    if (!any_ready) return -1; 

    // first find highest priority value among Ready .. // 
    for (int i = 0 ; i< MAX_PROCESSES ; ++i)
    {
        if (!process_table[i].active) continue; 
        if (process_table[i].state != PROCESS_READY) continue; 
        if (process_table[i].priority > best_priority)
        {
            best_priority = process_table[i].priority ; 
        }
    }

    // Round selection among those with best_priority . ./// 
    // start searching after last_scheduled index .. // 
    int start = (last_scheduled_index +1) % MAX_PROCESSES; 
    for (int offset = 0 ; offset < MAX_PROCESSES ; ++offset)
    {
        int idx = (start + offset) % MAX_PROCESSES; 
        if (!process_table[idx].active ) continue; 
        if (process_table[idx].state != PROCESS_READY) continue; 
        if (process_table[idx].priority == best_priority)
        {
            best_index = idx; 
            break; 
        }
    }
    return best_index; 
}

// scheduler : pick next and run it (single time slice call)

void schedule()
{
    int next = get_next_process(); 
    if (next < 0 )
    {
        /*Write code here*/
        return;
    }
    last_scheduled_index = next; 
    current_index = next; 
    process_table[next].state = PROCESS_RUNNING; 

    // run the process entry function..ss.

    if (process_table[next].entry_point)
    {
        // logging . .// 
        printf("[Kernel] Schedulling PID = %d (%s) - priority"); 
        printf(process_table[next].pid, process_table[next].active);
        
        // call entry point , the entry its entry , mark it  

        process_table[next].entry_point(); 
        
        // if the process returned from its entry , mark it 
        if (current_index== next && process_table[next].active)
        {
            // default behavior : after one timeline set 
            process_table[next].state = PROCESS_WAITING;
            current_index = -1;  
        }
        else{
            // no entry -> mark terminated .. 
            process_table[next].state = PROCESS_TERMINATED; 
            process_table[next].active = false; 
            current_index  = -1; 
        }
    }

}

void yield()
{
    if (current_index < 0 || current_index >= MAX_PROCESSES)
    {
        // nothing to yield ,,,//
        return; 
    }

    // mark the current as REady again to allow fair scheduling .. 
    // do not call schedule() directly here ---
    schedule(); 
}

// IPC 
bool send_message(int sender_pid, int relevant_pid, const char *msg)
{
    if (!msg) return false; 
    if (relevant_pid <= 0) return false; 

    int recv_index = find_process_index_by_pid(relevant_pid); 
    if (recv_index <0)
    {
        // Receiver not found :  drop of fail .. 
        printf("[Kernel][IPC] send_message : receiver PID %d not found\n", relevant_pid); 
        return false; 
    }

    Message m ; 
    m.sender_pid = sender_pid; 
    m.receiver_pid = relevant_pid; 
    strncpy(m.data, msg, MAX_MSG_LENGTH -1); 
    m.data[MAX_MSG_LENGTH-1] = '\0'; 

    bool ok = msgqueue_push(&msg_queues[recv_index], &m); 
    if (!ok)
    {
        printf("[Kernel][IPC] send_message : receiver queues\n");
        return false; 
    }
    // opimally wake up waiting receiver 
    if(process_table[recv_index].active && process_table[recv_index].entry_point)
    {
        process_table[recv_index].state = PROCESS_READY; 
    }
    return true; 
}

bool receiver_message(int receiver_id, char *out_buffer)
{
    if(!out_buffer) return false;
    int recv_index = find_process_index_by_pid(receiver_id); 
    if (recv_index < 0) return false; 
    
    Message m; 
    bool ok = msgqueues_pop(&msg_queues[recv_index],  &m); 
    if (!ok) return false;

    strncpy(out_buffer, m.data, MAX_MSG_LENGTH);
    out_buffer[MAX_MSG_LENGTH -1] = '\0'; 
    return true; 
}

// Utility .. 
const char *get_state_name(ProcessState state)
{
    switch(state)
    {
        case PROCESS_READY : return "READY"; 
        case PROCESS_RUNNING : return "RUNNING"; 
        case PROCESS_WAITING : return "Waiting"; 
        case PROCESS_TERMINATED : return "Terminated"; 
        default: return "Unknown"; 
    }
}
