#ifndef RULE_ENGINE_H
#define RULE_ENGINE_H

#include "plugin_manager.h"

// Kiểm tra plugin có được phép load không
bool rule_allow_plugin(const PluginInfo* plugin);

// Có thể thêm rule mới hoặc reload từ file JSON/YAML
void reload_rules();
void add_rule(const char* key, const char* condition);

#endif // RULE_ENGINE_H
