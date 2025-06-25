#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include "plugin_manager.h"

// Ghi log lỗi plugin
void log_crash_and_rollback(const PluginInfo* plugin);

// Rollback theo snapshot hoặc backup
bool rollback_plugin_state(const PluginInfo* plugin);

// Ghi danh sách plugin bị rollback
void record_failed_plugin(const char* name);

#endif // ROLLBACK_MANAGER_H
