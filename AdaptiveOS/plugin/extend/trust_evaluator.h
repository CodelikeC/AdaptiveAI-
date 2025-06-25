#ifndef TRUST_EVALUATOR_H
#define TRUST_EVALUATOR_H

#include "plugin_manager.h"

// Tính điểm tin cậy dựa trên metadata, signature, lịch sử
double compute_trust_score(const PluginInfo* plugin);

// Optional: cập nhật blacklist/whitelist
void add_to_blacklist(const char* plugin_name);
bool is_blacklisted(const char* plugin_name);

#endif // TRUST_EVALUATOR_H
