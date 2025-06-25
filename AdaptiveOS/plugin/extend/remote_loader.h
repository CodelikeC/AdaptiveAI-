#ifndef REMOTE_LOADER_H 
#define REMOTE_LOADER_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    LOAD_SUCCESS,  
    LOAD_FAILURE, 
    LOAD_TRUST_DENIED,
    LOAD_TIMEOUT, 
    LOAD_INVALID_SIGNATURE
    
} RemoteLoadStatus;

// Cau truc metadata mo ta plugin tu xa .. // 
typedef struct
{
    char plug_in_name[64];
    char source_ur[256]; 
    char checksum[64]; 
    char signature[256]; 
    int priority;  

} RemotePluginMeta;

// API chinh ..// 

RemoteLoadStatus download_and_verify_plugin(const RemotePluginMeta *remote);

// ham phat trien 
bool is_plugin_signed_correctly(const RemotePluginMeta *remote);
bool validate_plugin_checksum(const char *filepath, const char *expected_checksum); 
bool install_remote_plugin(const char *filepath); 
void log_remote_plugin_event(const RemotePluginMeta *meta, RemoteLoadStatus status );


#endif 