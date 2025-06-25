#ifndef PLUGIN_CLIENT_H 
#define PLUGIN_CLIENT_H 

#include <stdbool.h>

#define MAX_PLUGIN_NAME 64 
#define MAX_SERVER_ADDR 128 

typedef struct 
{
    char plugin_name[MAX_PLUGIN_NAME]; 
    char target_server[MAX_SERVER_ADDR];
    char auth_token[64]; 
    // Xac thuc : Authentication .. // 
    char plugin_version[16];
      
}PluginRequest;

typedef enum 
{
    CLIENT_OK,
    CLIENT_CONNECTION_FAILED, 
    CLIENT_SEND_ERROR, 
    CLIENT_RESPONSE_ERROR
}PluginClientStatus; 

// API chinh  ..
PluginClientStatus request_plugin_load(const PluginRequest *request);

// Tien ich .. // 
void print_plugin_request(const PluginRequest *request); 


#endif 