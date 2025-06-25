#include "rollback_manager.h"
#include <stdio.h>
#include <string.h>

void loag_crash_and_rollback (const PluginInfo *plugin)
{
    printf("Rollback - Plugin '%s' crashed"); 
    // ghi loi vao file log ..// 
    FILE *f = fopen("Rollback.log", "a"); 
    if (f)
    {
        fprintf(f, "plugin: %s | Version : %s | RolledBack\n", plugin -> name, plugin -> version ); 
        fclose(f); 
    }
}

bool rollback_plugin_state (const PluginInfo *plugin)
{
    // Mo phong hanh vi rollback tu snapshot .. // 
    printf("[Rollback] Plugin") ; 
    return true; 
}

void record_failed_plugin(const char *name)
{
    printf("[ROLLBACK] PLugin '%s' marked"); 
    // luu lai plugin loi vao file .. 
    FILE *f = fopen("Failed_plugins.txt", name); 
    if (f)
    {
        fprintf(f, "%s\n", name); 
        fclose(f); 
    }
}