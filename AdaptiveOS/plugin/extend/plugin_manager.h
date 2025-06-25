#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <stdbool.h>

typedef struct {
    char name[64];
    char version[16];
    char author[64];
    double trust_score;
    bool verified;
    bool sandboxed;
    char filepath[256];
} PluginInfo;

// Core functions
bool load_plugin(const char* filepath);
bool unload_plugin(const char* name);
void list_plugins();

// Metadata parser
bool parse_plugin_metadata(const char* filepath, PluginInfo* info);

#endif // PLUGIN_MANAGER_H




