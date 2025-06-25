#include "rule_engine.h"
#include <stdio.h>
#include <string.h>

bool validate_plugin_rules(const PluginInfo *plugin)
{
    printf("[RULE ENGINE] Validating rules");
    // Gia su chi chap nhan plug in co chu 

    if (strstr(plugin -> version, "trusted"))
    {
        return true;
    }
    return false; 
}

bool enforce_loading_policies(const PluginInfo *plugin)
{
    // Chinh sach : plugin khong duoc co ten .. // 
    const char *blacklist [] = {
        "hack", 
        "exploit", 
        "malware"
    }; 
    for ( int i = 0 ; i < 3 ; ++i){
        if (strstr(plugin -> name, blacklist[i] != NULL))
        {
            printf("[RULE ENGINE] Plugin '%s' violence loading privacy", plugin -> name); 
            return false; 
        }
    }
    return true ; 
}