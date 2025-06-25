#ifndef SANDBOX_H
#define SANDBOX_H

#include "plugin_manager.h"

// Load plugin trong môi trường tách biệt
bool load_into_sandbox(PluginInfo* plugin);
bool unload_from_sandbox(PluginInfo* plugin);

// Kiểm soát tài nguyên plugin được phép sử dụng
bool sandbox_limit_cpu(PluginInfo* plugin, double percent);
bool sandbox_limit_memory(PluginInfo* plugin, int mb);

#endif // SANDBOX_H
