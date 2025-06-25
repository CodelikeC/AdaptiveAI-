#ifndef KERNEL_H 
#define KERNEL_H 

#include <stdio.h>
#include <stdlib.h>

#include <string.h> 
#include <stdint.h>

#include <stdbool.h>

#define MAX_PROCESSES 128 
#define MAX_MSG_LENGTH 256 
#define MAX_QUEUE_SIZE 64 

// trang thai tien trinh .. / 
typedef enum 
{
    PROCESS_READY, 
    PROCESS_RUNNING,
    PROCESS_WAITING, 
    PROCESS_TERMINATED
}ProcessState; 

// cau truc tien trinh .. // 
typedef struct 
{
    int pid;
    char name [ 32]; 
    ProcessState state;
    int priority; 
    void (*entry_point) (void); // ham thuc thi 
    bool active;
} Process; 

// Cau truc thong diep IPC 
typedef struct 
{
    int sender_pid;
    int receiver_pid; 
    char data[MAX_MSG_LENGTH]; 
} Message; 

// Hang doi thong diep . . // 
typedef struct 
{
    Message queue[MAX_QUEUE_SIZE]; 
    int head; 
    int tail; 
    int count; 
} MessageQueue; 

// -- Quan ly tien trinh --- // 
void kernel_init(); 
int create_process(const char *name, int priority, void (*entry)(void));
void terminate_process(void); 
void print_process_table(); 

//-- lap lich -- // 
void schedule();
int get_next_process(); 
void yield(); 

// -- IPC -- // 
bool send_message(int sender_pid, int reciever_pid, const char *msg );
bool receive_message(int receiver_pid, char *out_buffer); 

// -- tien ich -- // 
const char * get_state_name(ProcessState state);

#endif
