#ifndef IPC_H 
#define IPC_H 

#include <stdio.h> 
#include <string.h> 

#define MAX_MESSAGES_LENGTH 128 
typedef struct 
{
    int from_pid; 
    int to_pid;
    char data[MAX_MESSAGES_LENGTH];
}Message; 

bool send_message(int from, int to, const char *data);  
bool receive_message(int to, char *buffer, int buffer_size );

#endif 