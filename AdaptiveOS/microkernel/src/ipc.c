#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>

#include "ipc.h"

#define MAX_MESSAGES 256 
#define MAX_MESSAGES_LENGTH 128 

typedef struct
{
    int from_pid;
    int to_pid; 
    char data [MAX_MESSAGES_LENGTH]; 
}Message;

static Message message_queue[MAX_MESSAGES]; 
static int message_count = 0; 

bool send_message(int from, int to, const char *data)
{
    if (message_count >= MAX_MESSAGES)
    {
        printf("[IPC] Message Queue full. Dropping message.\n"); 
        return 0; 
    }
    Message msg = {.from_pid = from, .to_pid = to};
    strncpy(msg.data, data, MAX_MESSAGES_LENGTH - 1); 
    message_queue[message_count++] = msg; 
    printf("[IPC] Message sent from PID = %d to PID = %d : %s", from, to, data ); 
    return 1; 
}

bool receive_message(int to, char *buffer, int buffer_size)
{
    for (int i = 0 ; i < message_count ; ++i)
    {
        if (message_queue[i].to_pid == to)
        {
            strncpy(buffer, message_queue[i].data, buffer_size);
            printf("[IPC] Message recieved by PID = %d : %s\n", to , buffer) ;
            // remove message from queue .. // 
            for (int j = i ; j < message_queue - 1 ; j++)
            {
                message_queue[j] = message_queue[j+1];
            }
            --message_count; 
            return 1; 
        }
    }
    return 0;
}